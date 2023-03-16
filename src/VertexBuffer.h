#pragma once

#include <stddef.h>
#include <GL/glew.h>

class VertexBuffer {
private:
    unsigned id;

public:
    VertexBuffer(void *data, size_t size, int mode = GL_STATIC_DRAW);
    VertexBuffer();
    ~VertexBuffer();

    void Generate();
    void UpdateData(void *data, size_t size, int mode = GL_STATIC_DRAW);
    void UpdateSubData(GLintptr offset, GLsizeiptr size, const void* data);
    void Bind() const;
    void Unbind() const;
};
