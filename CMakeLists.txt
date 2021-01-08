cmake_minimum_required(VERSION 3.0)
project(autoli)

include (cmake/utils.cmake)#查看cmake下的文件
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined ")
include_directories(.)
include_directories(/usr/local/include) 
link_directories(/usr/local/lib)
find_library(YAMLCPP yaml-cpp)
find_library(PTHREAD pthread)
message("***",${YAMLCPP})
set(LIB_SRC
        autoli/log.cc
        autoli/iomanager.cc
        autoli/util.cc
        autoli/config.cc
        autoli/thread.cc
        autoli/mutex.cc
        autoli/fiber.cc
        autoli/scheduler.cc
        autoli/timer.cc)

add_library(autoli SHARED  ${LIB_SRC})
force_redefine_file_macro_for_sources(autoli) #重定义__FILE__，以使用相对路径，这样日志使用相对路径

set(LIBS
        autoli
        pthread
        ${YAMLCPP})

add_executable(test tests/test.cc)
add_dependencies(test autoli)
force_redefine_file_macro_for_sources(test) #重定义__FILE__，以使用相对路径，这样日志使用相对路径
target_link_libraries(test ${LIBS})

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config autoli)
force_redefine_file_macro_for_sources(test_config) #重定义__FILE__，以使用相对路径，这样日志使用相对路径
target_link_libraries(test_config ${LIBS})#-L/usr/local/lib -lyaml-cpp)

add_executable(test_thread tests/test_thread.cc)
add_dependencies(test_thread autoli)
force_redefine_file_macro_for_sources(test_thread) #重定义__FILE__，以使用相对路径，这样日志使用相对路径
target_link_libraries(test_thread ${LIBS})#-L/usr/local/lib -lyaml-cpp)

add_executable(test_util tests/test_util.cc)
add_dependencies(test_util autoli)
force_redefine_file_macro_for_sources(test_util) #重定义__FILE__，以使用相对路径，这样日志使用相对路径
target_link_libraries(test_util ${LIBS})

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber autoli)
force_redefine_file_macro_for_sources(test_fiber) 
target_link_libraries(test_fiber ${LIBS})

add_executable(test_scheduler tests/test_scheduler.cc)
add_dependencies(test_scheduler autoli)
force_redefine_file_macro_for_sources(test_scheduler) 
target_link_libraries(test_scheduler ${LIBS})

add_executable(test_iomanager tests/test_iomanager.cc)
add_dependencies(test_iomanager autoli)
force_redefine_file_macro_for_sources(test_iomanager) 
target_link_libraries(test_iomanager ${LIBS})
SET(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
