#include "Shader.h"

#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

namespace {
    unsigned CompileShader(unsigned type, const char* Source);
    unsigned CreateShader(const char* vertexShader, const char* fragmentShader);
} // namespace

Shader::Shader()
{
}

Shader::Shader(const char* vertexShader, const char* fragmentShader)
{
    id = CreateShader(vertexShader, fragmentShader);
}

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
{
    id = CreateShader(vertexShader.c_str(), fragmentShader.c_str());
}

void Shader::Load(const char* vertexShader, const char* fragmentShader)
{
    id = CreateShader(vertexShader, fragmentShader);
}

void Shader::Load(const std::string& vertexShader, const std::string& fragmentShader)
{
    id = CreateShader(vertexShader.c_str(), fragmentShader.c_str());
}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::Bind() const
{
    glUseProgram(id);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform4f(const char* name, float v1, float v2, float v3, float v4)
{
    Bind();
    glUniform4f(GetUniformLocation(name), v1, v2, v3, v4);
}

void Shader::SetUniform3f(const char* name, float v1, float v2, float v3)
{
    Bind();
    glUniform3f(GetUniformLocation(name), v1, v2, v3);
}

void Shader::SetUniform1f(const char* name, float v)
{
    Bind();
    glUniform1f(GetUniformLocation(name), v);
}

void Shader::SetUniform1i(const char* name, int v)
{
    Bind();
    glUniform1f(GetUniformLocation(name), v);
}

void Shader::SetUniform4m(const char* name, const glm::mat4& model)
{
    Bind();
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &model[0][0]);
}

unsigned Shader::GetUniformLocation(const char* name)
{
    auto itr = shaderLocations.find(name);

    if (itr != shaderLocations.end()) {
        return itr->second;
    } else {
        return shaderLocations[name] = glGetUniformLocation(id, name);
    }
}

namespace {
    unsigned CompileShader(unsigned type, const char* source)
    {
        unsigned shaderId = glCreateShader(type);
        glShaderSource(shaderId, 1, &source, nullptr);
        glCompileShader(shaderId);

        int result;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE) {
            int lenght;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &lenght);
            auto error = (char*)alloca(lenght);
            glGetShaderInfoLog(shaderId, lenght, nullptr, error);
            std::cerr << "OPENGL::ERROR: Fail to compile "
                      << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:\n"
                      << error << '\n';
            glDeleteShader(shaderId);
            return 0;
        }

        return shaderId;
    }

    unsigned CreateShader(const char* vertexShader, const char* fragmentShader)
    {
        unsigned program = glCreateProgram();
        unsigned vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
        int result;

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &result);
        if (result == GL_FALSE) {
            int lenght;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &lenght);
            auto error = (char*)alloca(lenght);
            glGetProgramInfoLog(program, lenght, nullptr, error);
            std::cerr << "OPENGL::ERROR: Fail to link shader: " << error << '\n';
            glDeleteProgram(program);
            return 0;
        }

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }
} // namespace
