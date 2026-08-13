cmake_minimum_required(VERSION 3.28)
project(FPSGame LANGUAGES CXX C)

# need this for better distribution later on
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

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

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.3
)

FetchContent_MakeAvailable(glm)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Core)
add_subdirectory(Window)
add_subdirectory(Render)
# add_subdirectory(GameEngine)

add_executable(FPSGame
  Game/main.cpp
)

# stamp
set(ASSET_STAMP "${CMAKE_CURRENT_BINARY_DIR}/assets.stamp")

# require asset reload when rebuild
file(GLOB_RECURSE ASSET_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Game/assets/*")

add_custom_command(
  OUTPUT "${ASSET_STAMP}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/Game/assets"
    "$<TARGET_FILE_DIR:FPSGame>/assets"
  COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}/assets_$<CONFIG>.stamp"
  DEPENDS ${ASSET_FILES}
  COMMENT "Copying assets to the build directory..."
  VERBATIM
)

add_custom_target(copy_assets DEPENDS "${ASSET_STAMP}")
add_dependencies(FPSGame copy_assets)

target_link_libraries(FPSGame PUBLIC Core Window Render glm::glm) # glm is included throughout the entire project
