#include "ElementBuffer.h"

#include <GL/glew.h>

ElementBuffer::ElementBuffer(unsigned *indices, unsigned count, int mode)
    : count(count)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, mode);
}

ElementBuffer::ElementBuffer()
    : count(0)
{
}

void ElementBuffer::Generate()
{
    glGenBuffers(1, &id);
}

ElementBuffer::~ElementBuffer()
{
    glDeleteBuffers(1, &id);
}

void ElementBuffer::UpdateData(unsigned int *indices, unsigned int count, int mode)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, mode);
}

void ElementBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void ElementBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
