#include "logger.h"
#include "whereami.h"
#include <filesystem>
#include <initializer_list>
#include <path_helper.h>
#include <string>

using namespace Core;

std::filesystem::path PathHelper::currentDirectory = "/tmp/";

std::string PathHelper::getExecutablePath() {
    // should return the file location of the binary file
    const int lengthOfPath = wai_getExecutablePath(nullptr, 0, nullptr);
    if (lengthOfPath <= 0) {
        Logger::getInstance()->err(
            "FATAL. Couldn't get program path using whereami.");
        return "";
    }

    std::string buffer(static_cast<std::size_t>(lengthOfPath),
                       '\0'); // this auto gives a null terminator

    const int written = wai_getExecutablePath(
        buffer.data(), static_cast<int>(buffer.size()), nullptr);

    if (written <= 0) {
        Logger::getInstance()->err(
            "FATAL. Couldn't get program using whereami. Second step.");
        return "";
    }

    buffer.resize(static_cast<std::size_t>(written));

    return buffer;
}

void PathHelper::autoSetCurrentDirectory() {
    std::filesystem::path p = getExecutablePath();
    setCurrentDirectory(p.parent_path());
}

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
