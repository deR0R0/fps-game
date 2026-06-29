#pragma once

#include <string>

using namespace std;
namespace RenderLib {
class ShaderLoader {
public:
    static void loadShaders();

private:
    static string getShaderFromFile(string path);
    static unsigned int compileShader(string *shaderData, int SHADER_TYPE,
                                      string type = "UNKNOWN");
};
} // namespace RenderLib
