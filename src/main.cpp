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

int main() {
    // set the current directory!!!!
    filesystem::path mainExecutableFile(__FILE__);
    PathHelper::setCurrentDirectory(
        mainExecutableFile.parent_path()); // set to the parent path; this is
                                           // our game project root

    glfwSetErrorCallback(&glfwError);

    // init the windowlib to run glfwinit
    WindowLib::Window::init();
    GLFWwindow *window = WindowLib::Window::createWindow();

    RenderLib::ShaderLoader::loadShaders();

    // TODO: move all rendering logic to renderlib

    // square
    float vertices[] = {
        -0.5, -0.5, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f, 0.0f, -0.5f, 0.5f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,  1.0f, 0.5f, 0.5f, 0.0f, 0.0f,  0.0f, 1.0f,
        1.0f, 1.0f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f,
    };

    unsigned int indices[] = {0, 2, 1, 0, 3, 2};

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

    bool goingPositive = false;
    float scale = 0;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use shader program
        RenderLib::ShaderLoader::use();
        glUniform1f(uniId, scale);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        vao.bind();
        texture->bind();
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

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
