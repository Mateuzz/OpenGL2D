#include "Texture.h"

#include <GL/glew.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

Texture::Texture(const char* filename, bool alpha)
{
    stbi_set_flip_vertically_on_load(true); 

    int nChannels;
    int format = alpha ? GL_RGBA : GL_RGB;
    int req_comp = alpha ? 4 : 3;
    unsigned char* data = stbi_load(filename, &width, &height, &nChannels, req_comp);

    if (data) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture " << filename << std::endl;
    }
}

Texture::Texture()
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void Texture::Generate()
{
    glGenTextures(1, &id);
}

void Texture::Load(const char *filename, bool alpha)
{
    stbi_set_flip_vertically_on_load(true); 

    int nChannels;
    int format = alpha ? GL_RGBA : GL_RGB;
    int req_comp = alpha ? 4 : 3;
    unsigned char* data = stbi_load(filename, &width, &height, &nChannels, req_comp);

    if (data) {
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture " << filename << std::endl;
    }
}

void Texture::Bind(unsigned index) const
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind(unsigned index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, 0);
}
