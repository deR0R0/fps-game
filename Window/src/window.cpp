#include "window.h"
#include <cstddef>
#include <cstdlib>
#include <logger.h>

#include <GLFW/glfw3.h>

void WindowLib::Window::init() {
    if (!glfwInit()) {
        Core::Logger::getInstance()->err("ERROR: Couldn't init GLFW.");
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow *WindowLib::Window::createWindow(int width, int height,
                                            std::string windowName) {
    GLFWwindow *window =
        glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);
    if (!window) {
        Core::Logger::getInstance()->err("ERROR: Couldn't create window.");
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    Core::Logger::getInstance()->info("Successfully created window");

    return window;
}
