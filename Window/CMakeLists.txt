add_library(Window STATIC src/Window.cpp)

target_link_libraries(Window PUBLIC Core glfw)

target_include_directories(Window
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(Window PRIVATE -Wall -Wextra -Wpedantic -Werror)
