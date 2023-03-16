#include "BatchRenderer2D.h"
#include "util.h"
#include "ElementBuffer.h"
#include "config.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "BufferLayout.h"
#include "Shader.h"

#include <GL/glew.h>
#include <functional>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>
#include <iostream>
#include <stdint.h>
#include <algorithm>
#include <string>

namespace {
    constexpr size_t MaxQuadCount = 1000;
    constexpr size_t MaxVertexCount = 4 * MaxQuadCount;
    constexpr size_t MaxIndexCount = 6 * MaxQuadCount;

    std::string GetVertexShader()
    {
        std::ostringstream os;

        os << R"GLSL(

        #version 450 core

        layout(location = 0) in vec3 position;
        layout(location = 1) in vec2 textureCord;
        layout(location = 2) in vec4 color;
        layout(location = 3) in float textureIndex;

        out vec2 fragTextureCord;
        out vec4 fragColor;
        out float fragTextureIndex;

        uniform mat4 projection;

        void main()
        {
            fragTextureCord = textureCord;
            fragTextureIndex = textureIndex;
            fragColor = color;
            gl_Position = projection * vec4(position, 1.0f);
        }

        )GLSL";

        return os.str();
    }

    std::string GetFragmentShader()
    {
        std::ostringstream os;

        os << R"(

        #version 450 core

        out vec4 color;

        in vec2 fragTextureCord;
        in float fragTextureIndex;
        in vec4 fragColor;

        uniform sampler2D textureSampler[)" << std::to_string(config::ActualTexturesSlots) << "];";

        os << R"(

        void main()
        {
            int index = int(fragTextureIndex);
            color = texture(textureSampler[index], fragTextureCord) * fragColor;
        }

        )";

        return os.str();
    }

    struct Vertex {
        glm::vec3 positionCord;
        glm::vec2 textureCord;
        glm::vec4 colorRGB;
        float textureIndex;

        static Bufferlayout GenerateLayout() 
        {
            return {{GL_FLOAT, 3}, {GL_FLOAT, 2}, {GL_FLOAT, 4}, {GL_FLOAT, 1}};
        }
    };

    struct RendererData {
        Shader shader;
        int x;
        int y;
        int width;
        int height;
        glm::mat4 projection;

        VertexBuffer vbo;
        VertexArray vao;
        ElementBuffer ebo;

        Vertex* vertexData;
        Vertex* vertexDataPtr;

        std::array<unsigned, config::MaxTextureSlots> textureSlots;
        unsigned textureSlotIndex;

        unsigned indexCount;
    };

    RendererData gRendererData;
    BatchRenderer2D::Statistics gStats;
} // namespace

const BatchRenderer2D::Statistics& BatchRenderer2D::GetStats()
{
    return gStats;
}

void BatchRenderer2D::ResetStats()
{
    gStats = {};
}

void BatchRenderer2D::Initialize(int width, int height, int x, int y)
{
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &config::ActualTexturesSlots);
    config::ActualTexturesSlots = std::max(config::MaxTextureSlots, config::ActualTexturesSlots);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gRendererData.vertexData = new Vertex[MaxVertexCount];

    gRendererData.vao.Generate();
    gRendererData.vbo.Generate();
    gRendererData.ebo.Generate();

    gRendererData.vao.Bind();

    gRendererData.shader.Load(GetVertexShader(), GetFragmentShader());
    gRendererData.width = width;
    gRendererData.height = height;
    gRendererData.x = x;
    gRendererData.y = y;
    gRendererData.projection = glm::ortho<float>(x, width, height, y, -1.0f, 1.0f);

    gRendererData.vbo.UpdateData(gRendererData.vertexData, MaxVertexCount * sizeof(Vertex), GL_DYNAMIC_DRAW);
    gRendererData.vao.SetAttribute(gRendererData.vbo, Vertex::GenerateLayout());

    unsigned indices[MaxIndexCount];
    unsigned offset = 0;

    for (unsigned i = 0; i < MaxIndexCount; i += 6) {
        indices[i] = offset;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;

        indices[i + 3] = offset + 1;
        indices[i + 4] = offset + 2;
        indices[i + 5] = offset + 3;

        offset += 4;
    }

    gRendererData.ebo.UpdateData(indices, MaxIndexCount);
    gRendererData.shader.SetUniform4m("projection", gRendererData.projection);

    int *samplers = (int*)alloca(config::ActualTexturesSlots);

    for (int i = 0; i < config::ActualTexturesSlots; ++i) {
        samplers[i] = i;
    }

    glUniform1iv(glGetUniformLocation(gRendererData.shader.Id(), "textureSampler"), config::ActualTexturesSlots, samplers);
}

void BatchRenderer2D::Shutdown()
{
    delete[] gRendererData.vertexData;
}

void BatchRenderer2D::BeginBatch()
{
    gRendererData.vertexDataPtr = gRendererData.vertexData;
}

void BatchRenderer2D::EndBatch()
{
    GLsizeiptr size = (uint8_t*)gRendererData.vertexDataPtr - (uint8_t*)gRendererData.vertexData;
    gRendererData.vbo.UpdateSubData(0, size, gRendererData.vertexData);
}

void BatchRenderer2D::Flush()
{
    for (unsigned i = 0; i < gRendererData.textureSlotIndex; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, gRendererData.textureSlots[i]);
    }

    gRendererData.vao.Bind(); 
    gRendererData.shader.Bind();

    glDrawElements(GL_TRIANGLES, gRendererData.indexCount, GL_UNSIGNED_INT, nullptr);

    gStats.drawCalls++;
    gRendererData.indexCount = 0;
    gRendererData.textureSlotIndex = 0;
}

void BatchRenderer2D::DrawQuad(const glm::vec2 position, const glm::vec2& size, const Texture& texture)
{
    if (gRendererData.indexCount >= MaxIndexCount || gRendererData.textureSlotIndex >= (unsigned)config::ActualTexturesSlots) {
        EndBatch();
        Flush();
        BeginBatch();
    }      

    float textureIndex = -1;

    constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    constexpr glm::vec2 positionCords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}};
    constexpr glm::vec2 textureCords[] = {{0.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}};

    for (unsigned i = 0; i < gRendererData.textureSlotIndex; ++i) {
        if (texture.Id() == gRendererData.textureSlots[i]) {
            textureIndex = i;
            break;
        }
    }

    if (textureIndex < 0) {
        textureIndex = gRendererData.textureSlotIndex;
        gRendererData.textureSlots[gRendererData.textureSlotIndex] = texture.Id();
        gRendererData.textureSlotIndex++;
    }

    for (int i = 0; i < 4; ++i) {
        gRendererData.vertexData->positionCord = glm::vec4(positionCords[i].x * size.x + position.x, positionCords[i].y * size.y + position.y, 0.0f, 1.0f);
        gRendererData.vertexData->textureCord = textureCords[i];
        gRendererData.vertexData->colorRGB = color;
        gRendererData.vertexData->textureIndex = textureIndex;
        gRendererData.vertexDataPtr++;
    }

/*     gRendererData.vertexDataPtr->positionCord = glm::vec4(0.0 + position.x, 0.0f + position.y, 0.0f, 1.0f); */
/*     gRendererData.vertexDataPtr->textureCord = {0.0f, 1.0f}; */
/*     gRendererData.vertexDataPtr->colorRGB = color; */
/*     gRendererData.vertexDataPtr->textureIndex = textureIndex; */
/*     gRendererData.vertexDataPtr++; */

/*     gRendererData.vertexDataPtr->positionCord = glm::vec4(1.0f * size.x + position.x, 0.0f + position.y, 0.0f, 1.0f); */
/*     gRendererData.vertexDataPtr->textureCord = {1.0f, 1.0f}; */
/*     gRendererData.vertexDataPtr->colorRGB = color; */
/*     gRendererData.vertexDataPtr->textureIndex = textureIndex; */
/*     gRendererData.vertexDataPtr++; */

/*     gRendererData.vertexDataPtr->positionCord = glm::vec4{0.0f + position.x, 1.0f * size.y + position.y, 0.0f, 1.0f}; */
/*     gRendererData.vertexDataPtr->textureCord = {0.0f, 0.0f}; */
/*     gRendererData.vertexDataPtr->colorRGB = color; */
/*     gRendererData.vertexDataPtr->textureIndex = textureIndex; */
/*     gRendererData.vertexDataPtr++; */

/*     gRendererData.vertexDataPtr->positionCord = glm::vec4{1.0f * size.x + position.x, 1.0f * size.y + position.y, 0.0f, 1.0f}; */
/*     gRendererData.vertexDataPtr->textureCord = {1.0f, 0.0f}; */
/*     gRendererData.vertexDataPtr->colorRGB = color; */
/*     gRendererData.vertexDataPtr->textureIndex = textureIndex; */
/*     gRendererData.vertexDataPtr++; */

    gRendererData.indexCount += 6;
}

void BatchRenderer2D::DrawQuad(const glm::vec2 position,
                          const glm::vec2& size,
                          float radians,
                          const Texture& texture)
{
}

void BatchRenderer2D::DrawQuad(const glm::vec2 position,
                          const glm::vec2& size,
                          const glm::vec3& color,
                          const Texture& texture)
{
}

void BatchRenderer2D::DrawQuad(const glm::vec2 position,
                          const glm::vec2& size,
                          const glm::vec3& color,
                          float radians,
                          const Texture& texture)
{
}

void BatchRenderer2D::SetBlendDrawMode()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void BatchRenderer2D::SetDrawMode(int mode)
{
    glPolygonMode(GL_FRONT_AND_BACK, mode);
}

void BatchRenderer2D::ClearBuffer()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void BatchRenderer2D::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void BatchRenderer2D::SetViewport(int x, int y, int width, int height)
{
    gRendererData.width = width;
    gRendererData.height = height;
    gRendererData.projection = glm::ortho<float>(x, width, height, y, -1.0f, 1.0f);
    glViewport(x, y, width, height);
    gRendererData.shader.SetUniform4m("projection", gRendererData.projection);
}

void BatchRenderer2D::UpdateProjection(int x, int y, int width, int height)
{
    gRendererData.width = width;
    gRendererData.height = height;
    gRendererData.projection = glm::ortho<float>(x, width, height, y, -1.0f, 1.0f);
    gRendererData.shader.SetUniform4m("projection", gRendererData.projection);
}


