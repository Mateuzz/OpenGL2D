#pragma once

class Texture {
private:
    unsigned id;
    int width;
    int height;
    
public:
    Texture(const char* filename, bool alpha = true);
    Texture();
    ~Texture();
    
    void Generate();
    void Load(const char* filename, bool alpha = true);

    auto Width() const { return width; }
    auto Height() const { return height; }

    auto Id() const { return id; }

    void Bind(unsigned index = 0) const;
    static void Unbind(unsigned index = 0);
};
