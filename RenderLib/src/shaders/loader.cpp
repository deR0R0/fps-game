// This file loads shaders by file and loads into a shader program

#include "shaders/loader.h"
#include "glad/gl.h"

#include "GLFW/glfw3.h"
#include "logger.h"
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

using namespace Core;
using namespace RenderLib;
using namespace std;

// discovers every .glsl file in this file directory and loads them to a shader
// program
void ShaderLoader::loadShaders() {
    gladLoadGL(glfwGetProcAddress);

    // get current FILE directory (not working directory)
    filesystem::path shaderFileLoader(__FILE__);
    filesystem::path shaderDirectory = shaderFileLoader.parent_path();

    // compile vertex shader
    string vertexShaderSource = getShaderFromFile(
        "/Users/rorozee1/Documents/Programming "
        "Projects/C++/fps-game/RenderLib/src/shaders/vertex_shader.glsl");

    auto vertexShaderID =
        compileShader(&vertexShaderSource, GL_VERTEX_SHADER, "VERTEX");

    Logger::getInstance()->info(
        "After vertex shader compilation, vertex shader id is: " +
        to_string(vertexShaderID));

    // compile fragment shader
    string fragmentShaderSource = getShaderFromFile(
        "/Users/rorozee1/Documents/Programming "
        "Projects/C++/fps-game/RenderLib/src/shaders/fragment_shader.glsl");

    auto fragmentShaderID =
        compileShader(&fragmentShaderSource, GL_FRAGMENT_SHADER, "FRAGMENT");

    Logger::getInstance()->info(
        "After fragment shader compilation, fragment shader id is: " +
        to_string(fragmentShaderID));
    // iterate thru all files - FUTURE USE
    /*
    for (const auto &entry : filesystem::directory_iterator(shaderDirectory)) {
        if (entry.path().string().ends_with(".glsl")) {
            Logger::getInstance()->info("Found shader with path: " +
                                        entry.path().string());
            string shaderData = getShaderFromFile(entry.path().string());

            if(shaderData)
        }
    }
    */

    // import to a shader program
    int success;
    char log[512];

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShaderID);
    glAttachShader(shaderProgram, fragmentShaderID);
    glLinkProgram(shaderProgram);

    // errors?
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, log);
        Logger::getInstance()->err("Error linking shader program: " +
                                   string(log));
    } else {
        glUseProgram(shaderProgram);
    }

    // ensure shader objects are gone
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

unsigned int ShaderLoader::compileShader(string *shaderData, int SHADER_TYPE,
                                         string type) {
    // convert vertex data to a c styled string
    const char *shaderSource = shaderData->c_str();

    // compile
    Logger::getInstance()->info("Compiling Shader: " + type);

    // for if compilation failed
    int success;
    char infoLog[512];

    unsigned int shader;
    shader = glCreateShader(SHADER_TYPE);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // check for error logs during compilation
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        Logger::getInstance()->err("Failed to compile shader (" + type +
                                   "): " + string(infoLog));
        return 0;
    } else {
        Logger::getInstance()->info("Successfully compiled shader (" + type +
                                    ")");
        return shader; // return shader id
    }
}

string ShaderLoader::getShaderFromFile(string path) {
    // find the file
    filesystem::path p = filesystem::absolute(path);
    ifstream file(p);

    if (!file) {
        Logger::getInstance()->err(
            "Couldn't find shader file with given path: " + path);
        return "";
    }

    // stream the string into buffer and return the string
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
