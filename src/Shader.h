/***************************************************************************
*  @Description = A Shader from opengl graphics api, used for rendering
*  graphics inside the Renderer Class
****************************************************************************/

#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Shader {
private:
    unsigned id;
    std::unordered_map<const char*, unsigned> shaderLocations;

public:
    Shader();
    Shader(const char* vertexShader, const char* fragmentShader);
    Shader(const std::string& vertexShader, const std::string& fragmentShader);
    ~Shader();

    void Load(const char* vertexShader, const char* fragmentShader);
    void Load(const std::string& vertexShader, const std::string& fragmentShader);

    auto Id() const { return id; }

    bool IsLoaded() const { return id != 0; }

    void SetUniform4f(const char* name, float v1, float v2, float v3, float v4);
    void SetUniform3f(const char* name, float v1, float v2, float v3);
    void SetUniform1f(const char* name, float v);
    void SetUniform1i(const char* name, int i);
    void SetUniform4m(const char* name, const glm::mat4& model);
    void Bind() const;
    void Unbind() const;

    unsigned GetUniformLocation(const char* name);
};
