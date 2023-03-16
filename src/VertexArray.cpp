#include "VertexArray.h"

#include "BufferLayout.h"
#include "VertexBuffer.h"

#include <GL/glew.h>

VertexArray::VertexArray()
{
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &id);
}

void VertexArray::Generate()
{
    glGenVertexArrays(1, &id);
}

void VertexArray::Bind() const
{
    glBindVertexArray(id);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::SetAttribute(const VertexBuffer& vbo, const Bufferlayout& layout)
{
    Bind();
    vbo.Bind();

    const auto& attributes = layout.AtributesView();

    for (unsigned i = 0; i < layout.AttributesCount(); ++i) {
        const auto& attribute = attributes[i];

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i,
                              attribute.count,
                              attribute.type,
                              attribute.normalized,
                              layout.Stride(),
                              (const void*)attribute.offset);
    }
}
