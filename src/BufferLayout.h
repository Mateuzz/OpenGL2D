#pragma once

#include <array>
#include <GL/glew.h>
#include <assert.h>
#include <initializer_list>
#include <utility>

struct VertexAttribute {
    int type; 
    unsigned count;
    int normalized;
    unsigned offset;

    static unsigned GetSizeType(int type);
};

class Bufferlayout {
public:
    static constexpr unsigned MaxAtributes = 15;

private:
    std::array<VertexAttribute, MaxAtributes> attributes;
    unsigned attributesCount = 0;
    unsigned size = 0;

public:
    Bufferlayout() = default;
    Bufferlayout(std::initializer_list<std::pair<int, unsigned>> attributes);
    ~Bufferlayout() = default;

    const auto& AtributesView() const { return attributes; }
    auto AttributesCount() const { return attributesCount; }
    auto Stride() const { return size; }

    void AddAttribute(int type, unsigned count);
    void ResetAttributes();
};
