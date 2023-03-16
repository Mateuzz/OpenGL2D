#include "BufferLayout.h"

unsigned VertexAttribute::GetSizeType(int type)
{
    switch (type) {
        case GL_FLOAT:          return sizeof(GLfloat);
        case GL_UNSIGNED_INT:   return sizeof(GLuint);
        case GL_UNSIGNED_BYTE:  return sizeof(GLbyte);
        case GL_UNSIGNED_SHORT: return sizeof(GLshort);
        default:                return 0;
    }
}

Bufferlayout::Bufferlayout(std::initializer_list<std::pair<int, unsigned>> attributesList)
{
    for (auto itr = attributesList.begin(); itr < attributesList.end(); ++itr) {
        attributes[attributesCount++] = {itr->first, itr->second, false, size};
        size += VertexAttribute::GetSizeType(itr->first) * itr->second;
    }
}

void Bufferlayout::AddAttribute(int type, unsigned count)
{
    attributes[attributesCount++] = {type, count, false, size};
    size += VertexAttribute::GetSizeType(type) * count;
}

void Bufferlayout::ResetAttributes()
{
    attributesCount = size = 0;
}
