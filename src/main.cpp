#include "GLFW/glfw3.h"
#include "window.h"
#include <iostream>

static void glfwError(int id, const char *description) {
    std::cout << description << std::endl;
}

int main() {
    glfwSetErrorCallback(&glfwError);
    std::cout << "Hello from main file" << std::endl;
    // init the windowlib to run glfwinit
    WindowLib::Window::init();
    GLFWwindow *window = WindowLib::Window::create_window();
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
