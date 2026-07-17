#include <filesystem>
#include <initializer_list>
#include <path_helper.h>

using namespace Core;

std::filesystem::path PathHelper::currentDirectory = "/tmp/";

void PathHelper::setCurrentDirectory(std::filesystem::path dir) {
    currentDirectory = std::filesystem::absolute(dir);
}

std::filesystem::path PathHelper::getCurrentDirectory() {
    return currentDirectory;
}

std::filesystem::path
PathHelper::getResourcePath(std::initializer_list<std::string> list) {
    std::filesystem::path finalPath = currentDirectory;

    for (std::string path : list) {
        finalPath = finalPath / path;
    }

    return finalPath;
}
