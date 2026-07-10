#include "mesh/vbo.h"
#include <glad/gl.h>

// generates a buffer and then uploads it to opengl
VBO::VBO(float *vertices, std::size_t size) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }

void VBO::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VBO::deleteVBO() { glDeleteBuffers(1, &ID); }
