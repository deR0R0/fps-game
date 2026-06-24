add_library(Core STATIC src/logger.cpp)

target_include_directories(Core
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(Core PRIVATE -Wall -Wextra -Wpedantic -Werror)
