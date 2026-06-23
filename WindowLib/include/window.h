#pragma once

#include "GLFW/glfw3.h"
namespace WindowLib {
class Window {
public:
    static void init();
    static GLFWwindow *create_window();
};
} // namespace WindowLib
