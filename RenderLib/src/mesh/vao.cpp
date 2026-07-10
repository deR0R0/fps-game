#include "glad/gl.h"
#include <mesh/ebo.h>
#include <mesh/vao.h>

VAO::VAO() { glGenVertexArrays(1, &ID); }

void VAO::linkVBO(VBO &vbo, unsigned int layout) {
    vbo.bind();

    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(layout);

    vbo.unbind();
}

void VAO::bind() { glBindVertexArray(ID); }

void VAO::unbind() { glBindVertexArray(0); }

void VAO::deleteVAO() { glDeleteVertexArrays(1, &ID); }
