#include "camera/camera.h"
#include "GLFW/glfw3.h"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/geometric.hpp"

using namespace RenderLib;

Camera::Camera(int width, int height, glm::vec3 position) {
    this->width = width;
    this->height = height;
    this->position = position;

    // make macos cursor
    unsigned char pixels[4] = {0, 0, 0, 0};
    GLFWimage image;
    image.width = 1;
    image.height = 1;
    image.pixels = pixels;
    invisibleCursor = glfwCreateCursor(&image, 0, 0);
}

Camera::~Camera() {
    glfwDestroyCursor(invisibleCursor);
    invisibleCursor = NULL;
}

void Camera::matrix(float FOVdeg, float nearPlane, float farPlane,
                    unsigned int shaderID, const char *uniform) {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    view = glm::lookAt(position, position + yaw, pitch);
    proj = glm::perspective(glm::radians(FOVdeg), (float)(width / height),
                            nearPlane, farPlane);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, uniform), 1, GL_FALSE,
                       glm::value_ptr(proj * view));
}

void Camera::inputs(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += speed * yaw;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position += speed * -glm::normalize(glm::cross(yaw, pitch));
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position += speed * -yaw;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += speed * glm::normalize(glm::cross(yaw, pitch));
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position += speed * pitch;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        position += speed * -pitch;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed = 0.4f;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        speed = 0.1f;
    }

    // mouse controls
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // hide mouse (works for windows probs)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // hide mouse for macos
        glfwSetCursor(window, invisibleCursor);

        // looking logic
        if (firstClick) {
            firstClick = false;
            glfwSetCursorPos(window, (width / 2.0), (height / 2.0));
        }

        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotx = sensitivity * (float)(mouseY - (height / 2.0)) / height;
        float roty = sensitivity * (float)(mouseX - (height / 2.0)) / height;

        glm::vec3 newYaw = glm::rotate(yaw, glm::radians(-rotx),
                                       glm::normalize(glm::cross(yaw, pitch)));

        if (!(glm::angle(newYaw, pitch) <= glm::radians(5.0f)) or
            (glm::angle(newYaw, -pitch)) <= glm::radians(5.0f)) {
            yaw = newYaw;
        }

        yaw = glm::rotate(yaw, glm::radians(-roty), pitch);

        glfwSetCursorPos(window, (width / 2.0), (height / 2.0));
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursor(window, NULL);
        firstClick = true;
    }
}
