add_library(WindowLib STATIC src/Window.cpp)

target_include_directories(WindowLib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(WindowLib PRIVATE -Wall -Wextra -Wpedantic -Werror)
