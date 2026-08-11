#pragma once

#include "GLFW/glfw3.h"
#include <string>
namespace WindowLib {
class Window {
public:
    static void init();
    static GLFWwindow *createWindow(int width, int height,
                                    std::string windowName);
};
} // namespace WindowLib
