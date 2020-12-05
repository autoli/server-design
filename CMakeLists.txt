cmake_minimum_required(VERSION 2.8)
project(autoli)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")
set(LIB_SRC
        autoli/log.cc)
add_library(autoli  SHARED  ${LIB_SRC})
#add_library(autoli_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES(autoli_static PROPERTIES OUTPUT_NAME "autoli")
add_executable(test tests/test.cc)
add_dependencies(test autoli)
target_link_libraries(test autoli)

SET(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
