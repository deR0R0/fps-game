#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/gl.h>

#include "GLFW/glfw3.h"
#include "logger.h"
#include "path_helper.h"
#include "shaders/loader.h"
#include "textures/texture.h"
#include "window.h"
#include <string>

#include <mesh/ebo.h>
#include <mesh/vao.h>
#include <mesh/vbo.h>

using namespace Core;

static void glfwError(int id, const char *description) {
    Logger::getInstance()->err("Oops! There seems to be an error with glfw:");
    Logger::getInstance()->err(description);
}

static const int width = 1000;
static const int height = 1000;

int main() {
    // set the current directory!!!!
    filesystem::path mainExecutableFile(__FILE__);
    PathHelper::setCurrentDirectory(
        mainExecutableFile.parent_path()); // set to the parent path; this is
                                           // our game project root

    glfwSetErrorCallback(&glfwError);

    // init the windowlib to run glfwinit
    WindowLib::Window::init();
    GLFWwindow *window =
        WindowLib::Window::createWindow(width, height, "FPSGame");

    RenderLib::ShaderLoader::loadShaders();

    // TODO: move all rendering logic to renderlib

    // pyramid yes
    float vertices[] = {-0.5f, 0.0f, 0.5f,  0.83f, 0.70f, 0.44f, 0.0f, 0.0f,
                        -0.5f, 0.0f, -0.5f, 0.83f, 0.7f,  0.44f, 5.0f, 0.0f,
                        0.5f,  0.0f, -0.5f, 0.83,  0.70f, 0.44f, 0.0f, 0.0f,
                        0.5f,  0.0f, 0.5f,  0.83f, 0.70f, 0.44f, 5.0,  0.0f,
                        0.0f,  0.8f, 0.0f,  0.92f, 0.86f, 0.76f, 2.5f, 5.0f};

    unsigned int indices[] = {0, 1, 2, 0, 2, 3, 0, 1, 4,
                              1, 2, 4, 2, 3, 4, 3, 0, 4};

    VAO vao;
    vao.bind();

    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));

    vao.linkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
    vao.linkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float),
                   (void *)(3 * sizeof(float)));
    vao.linkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float),
                   (void *)(6 * sizeof(float)));
    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    RenderLib::Texture *texture = new RenderLib::Texture(
        PathHelper::getResourcePath({"assets", "textures", "Asphalt.jpg"}),
        GL_TEXTURE0);

    texture->bind();
    texture->changeSetting(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    texture->changeSetting(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    texture->changeSetting(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture->changeSetting(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    texture->unbind();

    unsigned int uniId =
        glGetUniformLocation(RenderLib::ShaderLoader::sProgram, "scale");

    unsigned int tex0Uni =
        glGetUniformLocation(RenderLib::ShaderLoader::sProgram, "tex0");
    RenderLib::ShaderLoader::use();
    glUniform1i(tex0Uni, 0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // set to wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float rotation = 0.0f;
    double prev = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // use shader program
        RenderLib::ShaderLoader::use();

        // 3d?
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        double currTime = glfwGetTime();
        if (currTime - prev >= 1 / 60) {
            rotation += 0.5f;
            prev = currTime;
        }

        model = glm::rotate(model, glm::radians(rotation),
                            glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)(width / height),
                                0.1f, 100.0f);

        int modelLoc =
            glGetUniformLocation(RenderLib::ShaderLoader::sProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc =
            glGetUniformLocation(RenderLib::ShaderLoader::sProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projLoc =
            glGetUniformLocation(RenderLib::ShaderLoader::sProgram, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        vao.bind();
        texture->bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int),
                       GL_UNSIGNED_INT, 0);

        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // cleanup
    RenderLib::ShaderLoader::deleteProgram();
    vao.deleteVAO();
    vbo.deleteVBO();
    ebo.deleteEBO();

    glfwTerminate();
    return 0;
}
