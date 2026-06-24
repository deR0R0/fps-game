cmake_minimum_required(VERSION 3.23)
project(FPSGame LANGUAGES CXX C)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Core)
add_subdirectory(WindowLib)
#add_subdirectory(RenderLib)
#add_subdirectory(GameEngine)FPSGame

add_executable(FPSGame
  src/main.cpp
)

target_link_libraries(FPSGame PUBLIC Core WindowLib glfw)
