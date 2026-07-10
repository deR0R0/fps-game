#pragma once

#include <cstddef>
class EBO {
public:
    unsigned int ID;
    EBO(unsigned int *indices, std::size_t size);

    void bind();
    void unbind();
    void deleteEBO();
};
