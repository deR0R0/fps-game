#include "window.h"
#include <iostream>

int main() {
    std::cout << "Hello from main file" << std::endl;
    auto window = new WindowLib::window();

    window->print();
    return 0;
}
