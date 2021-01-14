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
link_libraries(z)
set(LIB_SRC
        autoli/address.cc
        autoli/bytearray.cc
        autoli/log.cc
        autoli/http/http.cc
        autoli/http/http_parser.cc
        autoli/http/http11_parser.rl.cc
        autoli/http/httpclient_parser.rl.cc
        autoli/http/http_session.cc
        autoli/http/http_server.cc
        autoli/http/servlet.cc
        autoli/http/http_connection.cc
        
        autoli/iomanager.cc
        autoli/util.cc
        autoli/hook.cc
        autoli/config.cc
        autoli/thread.cc
        autoli/mutex.cc
        autoli/fiber.cc
        
        autoli/scheduler.cc
        autoli/stream.cc
        autoli/streams/socket_stream.cc
        autoli/streams/zlib_stream.cc

        autoli/timer.cc
        autoli/fd_manager.cc
        autoli/socket.cc
        autoli/tcp_server.cc
        autoli/uri.rl.cc)

add_library(autoli SHARED  ${LIB_SRC})
force_redefine_file_macro_for_sources(autoli) #重定义__FILE__，以使用相对路径，这样日志使用相对路径

set(LIBS
        autoli
        dl
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

add_executable(test_hook tests/test_hook.cc)
add_dependencies(test_hook autoli)
force_redefine_file_macro_for_sources(test_hook) 
target_link_libraries(test_hook ${LIBS})

add_executable(test_address tests/test_address.cc)
add_dependencies(test_address autoli)
force_redefine_file_macro_for_sources(test_address) 
target_link_libraries(test_address ${LIBS})

add_executable(test_socket tests/test_socket.cc)
add_dependencies(test_socket autoli)
force_redefine_file_macro_for_sources(test_socket) 
target_link_libraries(test_socket ${LIBS})

add_executable(test_bytearray tests/test_bytearray.cc)
add_dependencies(test_bytearray autoli)
force_redefine_file_macro_for_sources(test_bytearray) 
target_link_libraries(test_bytearray ${LIBS})

add_executable(test_http tests/test_http.cc)
add_dependencies(test_http autoli)
force_redefine_file_macro_for_sources(test_http) 
target_link_libraries(test_http ${LIBS})

add_executable(test_http_parser tests/test_http_parser.cc)
add_dependencies(test_http_parser autoli)
force_redefine_file_macro_for_sources(test_http_parser) 
target_link_libraries(test_http_parser ${LIBS})

add_executable(test_tcp_server tests/test_tcp_server.cc)
add_dependencies(test_tcp_server autoli)
force_redefine_file_macro_for_sources(test_tcp_server) 
target_link_libraries(test_tcp_server ${LIBS})

add_executable(echo_server examples/echo_server.cc)
add_dependencies(echo_server autoli)
force_redefine_file_macro_for_sources(echo_server) 
target_link_libraries(echo_server ${LIBS})

add_executable(echo_server_udp examples/echo_server_udp.cc)
add_dependencies(echo_server_udp autoli)
force_redefine_file_macro_for_sources(echo_server_udp) 
target_link_libraries(echo_server_udp ${LIBS})

add_executable(echo_udp_client examples/echo_udp_client.cc)
add_dependencies(echo_udp_client autoli)
force_redefine_file_macro_for_sources(echo_udp_client) 
target_link_libraries(echo_udp_client ${LIBS})


autoli_add_executable(test_http_server "tests/test_http_server.cc" autoli "${LIBS}")
autoli_add_executable(test_http_connection "tests/test_http_connection.cc" autoli "${LIBS}" )
autoli_add_executable(test_uri "tests/test_uri.cc" autoli "${LIBS}" )

SET(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
