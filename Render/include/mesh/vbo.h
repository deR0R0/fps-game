#pragma once

#include <cstddef>
#include <glad/gl.h>
class VBO {
public:
    unsigned int ID;
    VBO(float *vertices, GLsizeiptr size);

    void bind();
    void unbind();
    void deleteVBO();
};
