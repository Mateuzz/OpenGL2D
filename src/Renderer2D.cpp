#include "Renderer2D.h"
#include "util.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "BufferLayout.h"

/* #include <GL/glew.h> */
/* #include <functional> */
#include <glm/gtc/matrix_transform.hpp>

namespace {
    struct RendererData {
        Shader shader;
        VertexBuffer vbo;
        VertexArray vao;
        int x;
        int y;
        int width;
        int height;
        glm::mat4 projection;
    };

    RendererData gRendererData;
} // namespace

void Renderer2D::Initialize(int width, int height, int x, int y)
{
    auto vertexShader = util::FromFile("shaders/vertex.shader");
    auto fragmentShader = util::FromFile("shaders/fragment.shader");

    gRendererData.vbo.Generate();
    gRendererData.vao.Generate();
    gRendererData.width = width;
    gRendererData.height = height;
    gRendererData.x = x;
    gRendererData.y = y;
    gRendererData.shader.Load(vertexShader, fragmentShader);

    gRendererData.projection = glm::ortho<float>(x, width, height, y, -1.0f, 1.0f);

    float positions[] = {
        // position        // texture position
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    gRendererData.vbo.UpdateData(positions, sizeof(positions));
    gRendererData.vao.SetAttribute(gRendererData.vbo, {{GL_FLOAT, 3}, {GL_FLOAT, 2}});

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gRendererData.shader.SetUniform4m("projection", gRendererData.projection);
}

void Renderer2D::Shutdown()
{
}

void Renderer2D::BeginScene()
{
    gRendererData.vao.Bind();
    gRendererData.shader.Bind();
}

void Renderer2D::SetBlendDrawMode()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer2D::SetDrawMode(int mode)
{
    glPolygonMode(GL_FRONT_AND_BACK, mode);
}

void Renderer2D::ClearBuffer()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer2D::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void Renderer2D::SetViewport(int x, int y, int width, int height)
{
    gRendererData.width = width;
    gRendererData.height = height;
    gRendererData.projection = glm::ortho<float>(x, width, height, y, -1.0f, 1.0f);
    glViewport(x, y, width, height);
    gRendererData.shader.SetUniform4m("projection", gRendererData.projection);
}

void Renderer2D::UpdateProjection(int x, int y, int width, int height)
{
    gRendererData.width = width;
    gRendererData.height = height;
    gRendererData.projection = glm::ortho<float>(x, width, height, y, -1.0f, 1.0f);
    gRendererData.shader.SetUniform4m("projection", gRendererData.projection);
}

void Renderer2D::DrawQuad(const glm::vec2 position, const glm::vec2& size, const Texture& texture)
{
    texture.Bind();

    glm::mat4 model = glm::mat4(1.0f) * 
                      glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) *
                      glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

    gRendererData.shader.SetUniform3f("ourColor", 1.0f, 1.0f, 1.0f);
    gRendererData.shader.SetUniform4m("model", model);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer2D::DrawQuad(const glm::vec2 position,
                          const glm::vec2& size,
                          float radians,
                          const Texture& texture)
{
    texture.Bind();

    glm::mat4 model = glm::mat4(1.0f);

    model *= glm::translate(model, glm::vec3(position, 0.0f)) *
             glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)) *
             glm::rotate(model, radians, glm::vec3(0.0f, 0.0f, 1.0f)) *
             glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)) *
             glm::scale(model, glm::vec3(size, 1.0f));

    gRendererData.shader.SetUniform3f("ourColor", 1.0f, 1.0f, 1.0f);
    gRendererData.shader.SetUniform4m("model", model);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer2D::DrawQuad(const glm::vec2 position,
                          const glm::vec2& size,
                          const glm::vec3& color,
                          const Texture& texture)
{
    texture.Bind();

    /* glm::vec3 center(400.0f, 300.0f, 0.0f); */
    /* glm::mat4 view = glm::translate(glm::mat4(1.0f), center) * */
    /*                     scale * */
    /*                     glm::translate(glm::mat4(1.0f), -center); */

    glm::mat4 model = glm::mat4(1.0f) * glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) *
                      glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

    gRendererData.shader.SetUniform4m("model", model);
    gRendererData.shader.SetUniform3f("ourColor", color.r, color.g, color.b);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer2D::DrawQuad(const glm::vec2 position,
                          const glm::vec2& size,
                          const glm::vec3& color,
                          float radians,
                          const Texture& texture)
{
    texture.Bind();

    glm::mat4 model = glm::mat4(1.0f);

    model *= glm::translate(model, glm::vec3(position, 0.0f)) *
             glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)) *
             glm::rotate(model, radians, glm::vec3(0.0f, 0.0f, 1.0f)) *
             glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)) *
             glm::scale(model, glm::vec3(size, 1.0f));

    gRendererData.shader.SetUniform4m("model", model);
    gRendererData.shader.SetUniform3f("ourColor", color.r, color.g, color.b);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
