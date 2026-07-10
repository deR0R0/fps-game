#pragma once

#include <cstddef>
class VBO {
public:
    unsigned int ID;
    VBO(float *vertices, std::size_t size);

    void bind();
    void unbind();
    void deleteVBO();
};
