add_library(Core STATIC
  whereami/src/whereami.c 

  src/logger.cpp
  src/path_helper.cpp
)

target_include_directories(Core
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/whereami/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src
)

target_compile_options(Core PRIVATE -Wall -Wextra -Wpedantic -Werror)
