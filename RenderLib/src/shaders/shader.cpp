// This file loads shaders by file and loads into a shader program
// Note: the included shaders in this file are backups, in case the game shaders
// can't load properly;

#include "shaders/shader.h"
#include "glad/gl.h"

#include "GLFW/glfw3.h"
#include "logger.h"
#include "path_helper.h"
#include <cstddef>
#include <fstream>
#include <sstream>
#include <string>

using namespace Core;
using namespace RenderLib;

void Shader::init() {
    gladLoadGL(glfwGetProcAddress);
} // stupid, yes. works? yes

Shader::Shader(const std::string &shaderName) {
    gladLoadGL(glfwGetProcAddress);
    Logger::getInstance()->info("Attempting to discover shaders \"" +
                                shaderName + "\"");

    std::string vertexShaderSource, fragShaderSource;
    unsigned int vertexShaderID, fragShaderID, programID;
    std::filesystem::path shaderDirectory = PathHelper::getCurrentDirectory();

    // get the source files
    vertexShaderSource =
        getShaderFromFile(shaderDirectory / (shaderName + ".vert"));
    fragShaderSource =
        getShaderFromFile(shaderDirectory / (shaderName + ".frag"));

    if (vertexShaderSource == "" || fragShaderSource == "") {
        Logger::getInstance()->warn(
            "Stopped creation of shader program because a vertex or fragment "
            "shader is missing for " +
            shaderName);
        return;
    }

    // compile the shaders
    vertexShaderID =
        compileShader(vertexShaderSource, GL_VERTEX_SHADER, "VERTEX");
    fragShaderID =
        compileShader(fragShaderSource, GL_FRAGMENT_SHADER, "FRAGMENT");

    if (vertexShaderID == 0 || fragShaderID == 0) {
        Logger::getInstance()->warn(
            "Couldn't create shader program due to shader compile fail for " +
            shaderName);

        // ensure we cleanup the shaders that may or may not have compiled
        if (vertexShaderID != 0) {
            glDeleteShader(vertexShaderID);
        }

        if (fragShaderID != 0) {
            glDeleteShader(fragShaderID);
        }
        return;
    }

    // create the shader program
    programID = compileProgram(vertexShaderID, fragShaderID);

    if (programID != 0) {
        Logger::getInstance()->info("Successfully created shader program \"" +
                                    shaderName +
                                    "\" with ID: " + std::to_string(programID));
        this->ID = programID;
    } else {
        Logger::getInstance()->warn("Shader program linking failed for " +
                                    shaderName);
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragShaderID);
}

Shader::~Shader() { this->destroy(); }

void Shader::use() {
    if (this->ID != 0) {
        glUseProgram(this->ID);
    }
}

void Shader::destroy() {
    if (this->ID != 0) {
        glDeleteProgram(this->ID);
        this->ID = 0;
    }
}

// discovers every .glsl file in this file directory and loads them to a shader
// program
unsigned int Shader::compileProgram(unsigned int vertx, unsigned int frag) {
    // import to a shader program
    int success;
    char log[512];

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertx);
    glAttachShader(shaderProgram, frag);
    glLinkProgram(shaderProgram);

    // errors?
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, log);
        Logger::getInstance()->err("Error linking shader program: " +
                                   string(log));
        // ensure the shader program is actually gone
        glDeleteProgram(shaderProgram);
    } else {
        return shaderProgram;
    }

    // ensure shader objects are gone
    return 0;
}

unsigned int Shader::compileShader(string &shaderData, int SHADER_TYPE,
                                   string type) {
    // convert vertex data to a c styled string
    const char *shaderSource = shaderData.c_str();

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

std::string Shader::getShaderFromFile(std::filesystem::path shaderFilePath) {
    // find the file
    std::filesystem::path p = filesystem::absolute(shaderFilePath);
    ifstream file(p);

    if (!file) {
        Logger::getInstance()->err(
            "Couldn't find shader file with given path: " +
            shaderFilePath.string());
        return "";
    }

    // stream the string into buffer and return the string
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
