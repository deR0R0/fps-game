#pragma once

#include <filesystem>
namespace RenderLib {
class Texture {
public:
    Texture(std::filesystem::path texturePath);
    ~Texture();

    // image stuff
    unsigned char *getBytes();
    int getWidth();
    int getHeight();
    int getColorChannelCount();

    // opengl stuff
    unsigned int getID();
    void bind();
    void unbind();
    void deleteTexture();
    void changeSetting(GLenum dimension, GLenum setting, GLenum value);

private:
    int width;
    int height;
    int colorChannelCount;
    unsigned char *bytes;

    unsigned int ID;
};
} // namespace RenderLib
