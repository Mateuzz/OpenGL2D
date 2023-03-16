#pragma once

#include <GL/glew.h>

class ElementBuffer {
private:
    unsigned id;
    unsigned count;

public:
    ElementBuffer();
    ElementBuffer(unsigned *indices, unsigned count, int mode = GL_STATIC_DRAW);
    ~ElementBuffer();

    void Generate();
    void UpdateData(unsigned* indices, unsigned count, int mode = GL_STATIC_DRAW);

    auto Count() const { return count; }

    void Bind() const;
    void Unbind() const;
};
