#include "glad/gl.h"
#include "logger.h"
#include <stb_image.h>
#include <textures/texture.h>

using namespace RenderLib;

// assume 2d textures for now.
Texture::Texture(std::filesystem::path texturePath) {
    // load the texture into ram
    stbi_set_flip_vertically_on_load(true);
    bytes =
        stbi_load(texturePath.c_str(), &width, &height, &colorChannelCount, 0);

    if (!bytes) {
        Core::Logger::getInstance()->err(
            "Issue loading a texture with the path: " + texturePath.string());
        Core::Logger::getInstance()->err(stbi_failure_reason());
    }

    // generate the texture object in opengl
    glGenTextures(1, &ID);
    bind();

    // upload to the gpu. we keep the texture bytes on the cpu side
    // for now.
    // TODO: remove texture from cpu after gpu upload

    // determine whetehr to use rgb or rgba depending on jpg or png
    GLenum rgbType = (colorChannelCount == 4) ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, rgbType, width, height, 0, rgbType,
                 GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture() {
    // free our bytes using stbi and then unbind + delete the texture
    stbi_image_free(bytes);
    unbind();
    deleteTexture();
}

void Texture::changeSetting(GLenum dimension, GLenum setting, GLenum value) {
    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(dimension, setting, value);
}

void Texture::bind() {
    glActiveTexture(
        GL_TEXTURE0); // temp position for now; will dynamically gen in a little
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::deleteTexture() { glDeleteTextures(1, &ID); }

unsigned int Texture::getID() { return ID; }

// image stuff
unsigned char *Texture::getBytes() { return bytes; }
int Texture::getWidth() { return width; }
int Texture::getHeight() { return height; }
int Texture::getColorChannelCount() { return colorChannelCount; }
