#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/gl.h>

#include "GLFW/glfw3.h"
#include "logger.h"
#include "shaders/loader.h"
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

int main() {
    glfwSetErrorCallback(&glfwError);

    // init the windowlib to run glfwinit
    WindowLib::Window::init();
    GLFWwindow *window = WindowLib::Window::createWindow();

    RenderLib::ShaderLoader::loadShaders();

    // TODO: move all rendering logic to renderlib

    // render triangle
    float vertices[] = {-0.5f,     -0.5f * float(sqrt(3)) / 3,    0.0f, // 0
                        0.5f,      -0.5f * float(sqrt(3)) / 3,    0.0f, // 1
                        0.0f,      0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // 1
                        -0.5f / 2, 0.5f * float(sqrt(3)) / 6,     0.0f,
                        0.5f / 2,  0.5f * float(sqrt(3)) / 6,     0.0f,
                        0.0f,      -0.5f * float(sqrt(3)) / 3,    0.0f};

    unsigned int indices[] = {0, 3, 5, 3, 2, 4, 5, 4, 1};

    VAO vao;
    vao.bind();

    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));

    vao.linkVBO(vbo, 0);
    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // set to wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        vao.bind();
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        RenderLib::ShaderLoader::use();
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
