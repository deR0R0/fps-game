#pragma once

#include <cstddef>
#include <glad/gl.h>
class EBO {
public:
    unsigned int ID;
    EBO(unsigned int *indices, GLsizeiptr size);

    void bind();
    void unbind();
    void deleteEBO();
};
