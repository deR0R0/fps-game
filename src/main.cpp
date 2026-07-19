#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include <glad/gl.h>

#include "camera/camera.h"
#include "glm/ext/vector_float3.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

static void swapVsync(bool enable) {
    Logger::getInstance()->info("Turning vsync to: " + std::to_string(enable));
    glfwSwapInterval(enable);
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

    double msFromPrevFrame = 0.0f;
    double fps = 0.0f;
    int framesPast = 0;
    double prevFPSTime = glfwGetTime();
    double timeForPrevFrame = glfwGetTime();
    bool shouldSwapVsync = true;

    glEnable(GL_DEPTH_TEST);

    // camera :O
    RenderLib::Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    std::string renderer(
        reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
    Logger::getInstance()->info("OpenGL renderer: " + renderer);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // use shader program
        RenderLib::ShaderLoader::use();

        // cam matrix
        camera.inputs(window);
        camera.matrix(45.0f, 0.1f, 100.0f, RenderLib::ShaderLoader::sProgram,
                      "camMatrix");

        // time related stuff
        double currTime = glfwGetTime();

        // frame time
        msFromPrevFrame = (currTime - timeForPrevFrame) * 1000;
        timeForPrevFrame = currTime;

        // fps calculation
        framesPast++;
        if (currTime - prevFPSTime >= 1.0) {
            fps = double(framesPast) / (currTime - prevFPSTime);
            framesPast = 0;
            prevFPSTime = currTime;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // draw actual images
        vao.bind();
        texture->bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int),
                       GL_UNSIGNED_INT, 0);

        // gui - title
        ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_FirstUseEver);
        ImGui::Begin("Debugging");

        // gui - content
        std::string frameTimeText =
            "Frame: " + std::to_string(msFromPrevFrame) + "ms";
        std::string fpsText = "FPS: " + std::to_string(fps);
        ImGui::TextUnformatted(frameTimeText.c_str());
        ImGui::TextUnformatted(fpsText.c_str());
        if (ImGui::Checkbox("Enable VSync", &shouldSwapVsync)) {
            swapVsync(shouldSwapVsync);
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfwSwapInterval(0);
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
