add_library(GameEngine STATIC
  vendor/stb/src/stb_image.cpp
)

target_link_libraries(GameEngine PUBLIC Core WindowLib RenderLib)

target_include_directories(GameEngine
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(GameEngine PRIVATE -Wall -Wextra -Wpedantic -Werror)
