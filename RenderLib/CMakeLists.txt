add_library(RenderLib STATIC vendor/glad/src/gl.c)

target_link_libraries(RenderLib PUBLIC Core WindowLib)

target_include_directories(RenderLib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/src

  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(RenderLib PRIVATE -Wall -Wextra -Wpedantic -Werror)
