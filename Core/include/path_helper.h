#pragma once

#include <filesystem>
#include <string>
namespace Core {
class PathHelper {
public:
    static void setCurrentDirectory(std::filesystem::path dir);
    static std::filesystem::path getCurrentDirectory();

    // other stuff
    static std::filesystem::path
    getResourcePath(std::initializer_list<std::string> list);

private:
    static std::filesystem::path currentDirectory;
};
} // namespace Core
