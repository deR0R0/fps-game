#pragma once

#include <string>

using namespace std;
namespace RenderLib {
class ShaderLoader {
public:
    static void loadShaders();
    static void use();
    static void deleteProgram();
    static unsigned int sProgram;

private:
    static string getShaderFromFile(string path);
    static unsigned int compileShader(string *shaderData, int SHADER_TYPE,
                                      string type = "UNKNOWN");
};
} // namespace RenderLib
