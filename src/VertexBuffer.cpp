#include "VertexBuffer.h"

#include <GL/glew.h>

VertexBuffer::VertexBuffer(void *data, size_t size, int mode)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, mode);
}

VertexBuffer::VertexBuffer()
{
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &id);
}

void VertexBuffer::Generate()
{
    glGenBuffers(1 ,&id);
}

void VertexBuffer::UpdateData(void *data, size_t size, int mode)
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, mode);
}

void VertexBuffer::UpdateSubData(GLintptr offset, GLsizeiptr size, const void *data)
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
