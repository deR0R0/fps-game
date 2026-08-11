#pragma once

#include <filesystem>
#include <string>

using namespace std;
namespace RenderLib {
class Shader {
public:
    static void init();

    Shader(const std::string &shaderName);
    ~Shader();

    // destroy copy semantics
    // (because if we copy, then delete one shader program, it will result bad)
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    void use();
    void destroy();

private:
    unsigned int ID = 0;

    static std::string getShaderFromFile(std::filesystem::path shaderFilePath);
    static unsigned int compileShader(string &shaderData, int SHADER_TYPE,
                                      string type = "UNKNOWN");
    static unsigned int compileProgram(unsigned int vertx, unsigned int frag);
};
} // namespace RenderLib
