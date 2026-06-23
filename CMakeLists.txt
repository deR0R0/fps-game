cmake_minimum_required(VERSION 3.23)
project(FPSGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(WindowLib)
#add_subdirectory(RenderLib)
#add_subdirectory(GameEngine)FPSGame

add_executable(FPSGame
  src/main.cpp
)

target_link_libraries(FPSGame PRIVATE WindowLib)
