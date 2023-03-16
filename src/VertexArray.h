#pragma once

class VertexBuffer;
class Bufferlayout;

class VertexArray {
private:
    unsigned id;

public:
    VertexArray();
    ~VertexArray();

    void Generate();
    void Bind() const;
    void Unbind() const;
    
    void SetAttribute(const VertexBuffer& vbo, const Bufferlayout& layout);
};
