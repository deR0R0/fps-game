#pragma once

#include <mesh/vbo.h>

class VAO {
public:
    unsigned int ID;
    VAO();

    void linkVBO(VBO &vbo, unsigned int layout);
    void bind();
    void unbind();
    void deleteVAO();
};
