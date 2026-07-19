add_library(RenderLib STATIC
  vendor/glad/src/gl.c

  vendor/imgui/src/imgui.cpp
  vendor/imgui/src/imgui_demo.cpp
  vendor/imgui/src/imgui_draw.cpp
  vendor/imgui/src/imgui_impl_glfw.cpp
  vendor/imgui/src/imgui_impl_opengl3.cpp
  vendor/imgui/src/imgui_tables.cpp
  vendor/imgui/src/imgui_widgets.cpp

  vendor/stb/src/stb_image.cpp

  src/shaders/loader.cpp
  src/mesh/vbo.cpp
  src/mesh/ebo.cpp
  src/mesh/vao.cpp
  src/textures/texture.cpp
  src/camera/camera.cpp
)

target_link_libraries(RenderLib PUBLIC Core WindowLib glm::glm)

target_include_directories(RenderLib
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(RenderLib PRIVATE -Wall -Wextra -Wpedantic -Werror)
