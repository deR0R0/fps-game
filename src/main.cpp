#include "GLFW/glfw3.h"
#include "logger.h"
#include "window.h"
#include <iostream>

using namespace Core;

static void glfwError(int id, const char *description) {
    std::cout << description << std::endl;
}

int main() {
    glfwSetErrorCallback(&glfwError);

    Logger::getInstance()->info("Test Info Logger");
    Logger::getInstance()->warn("Test Warn Logger");
    Logger::getInstance()->err("Test Error Logger");

    // init the windowlib to run glfwinit
    WindowLib::Window::init();
    GLFWwindow *window = WindowLib::Window::create_window();
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
