#pragma once

#include "glad/gl.h"
#include <mesh/vbo.h>

class VAO {
public:
    unsigned int ID;
    VAO();

    void linkAttrib(VBO &vbo, GLuint layout, GLuint numComponents, GLenum type,
                    GLsizeiptr stride, void *offset);
    void bind();
    void unbind();
    void deleteVAO();
};
