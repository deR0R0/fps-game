#include "GLFW/glfw3.h"
#include "logger.h"
#include "window.h"
#include <string>

using namespace Core;

static void glfwError(int id, const char *description) {
    Logger::getInstance()->err("Oops! There seems to be an error with glfw:");
    Logger::getInstance()->err(description);
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
