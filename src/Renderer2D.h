#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

class Renderer2D {
public:
    static void Initialize(int width, int height, int x, int y);
    static void Shutdown();

    static void SetBlendDrawMode();
    static void BeginScene();
    static void DrawQuad(const glm::vec2 position, const glm::vec2& size, const Texture &texture);
    static void DrawQuad(const glm::vec2 position, const glm::vec2& size, float radians, const Texture &texture);
    static void DrawQuad(const glm::vec2 position, const glm::vec2& size, const glm::vec3& color, const Texture &texture);
    static void DrawQuad(const glm::vec2 position, const glm::vec2& size, const glm::vec3& color, float radians, const Texture &texture);
    static void SetClearColor(float r, float g, float b, float a);
    static void SetDrawMode(int mode);
    static void ClearBuffer();
    static void SetViewport(int x, int y, int width, int height);
    static void UpdateProjection(int x, int y, int width, int height);
};
