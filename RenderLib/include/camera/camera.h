#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define GL_SILENCE_DEPRECATION

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/trigonometric.hpp>

namespace RenderLib {
class Camera {
public:
    // victor gordan's tutorial used orientation and up
    // but i think pitch and yaw is correct here.
    glm::vec3 position;
    glm::vec3 yaw = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 pitch = glm::vec3(0.0f, 1.0f, 0.0f);

    bool firstClick = false;

    int width; // TODO: encapsulate this and make getter/setter methods for when
               // the window resizes
    int height;

    float speed = 0.1f;
    float sensitivity = 100.0f;

    Camera(int width, int height, glm::vec3 position);
    ~Camera();

    void matrix(float FOVdeg, float nearPlane, float farPlane,
                unsigned int shaderID, const char *uniform);
    void inputs(GLFWwindow *window);

private:
    GLFWcursor *invisibleCursor;
};
} // namespace RenderLib
