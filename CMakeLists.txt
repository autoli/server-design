cmake_minimum_required(VERSION 3.0)
project(autoli)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/yaml-cpp)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/jsoncpp-1.8.4)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/sqlite-3.28.0)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/tinyxml2-7.0.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

include_directories(.)
include_directories(/usr/local)
include_directories(/usr/local/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/yaml-cpp/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/jsoncpp-1.8.4/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/sqlite-3.28.0)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/tinyxml2-7.0.1)
link_directories(/usr/local/lib)
link_directories(/usr/local/lib64)
link_directories(/usr/local/lib/hiredis-vip)

option(BUILD_TEST "ON for complile test" OFF)

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package(MySQL REQUIRED)
if(MYSQL_FOUND)
    include_directories(${MYSQL_INCLUDE_DIR})
endif()

find_package(Protobuf)
if(Protobuf_FOUND)
    include_directories(${Protobuf_INCLUDE_DIRS})
endif()
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIR})
endif()

set(LIB_SRC
    autoli/address.cc
    autoli/bytearray.cc
    autoli/config.cc
    autoli/db/fox_thread.cc
    autoli/db/mysql.cc
    autoli/db/redis.cc
    autoli/db/sqlite3.cc
    autoli/ds/bitmap.cc
    autoli/ds/roaring_bitmap.cc
    autoli/ds/roaring.c
    autoli/ds/util.cc
    autoli/email/email.cc
    autoli/email/smtp.cc
    autoli/env.cc
    autoli/daemon.cc
    autoli/fd_manager.cc
    autoli/fiber.cc
    autoli/http/http.cc
    autoli/http/http_connection.cc
    autoli/http/http_parser.cc
    autoli/http/http_session.cc
    autoli/http/http_server.cc
    autoli/http/servlet.cc
    autoli/http/servlets/config_servlet.cc
    autoli/http/servlets/status_servlet.cc
    autoli/http/session_data.cc
    autoli/http/ws_connection.cc
    autoli/http/ws_session.cc
    autoli/http/ws_server.cc
    autoli/http/ws_servlet.cc
    autoli/hook.cc
    autoli/iomanager.cc
    autoli/library.cc
    autoli/log.cc
    autoli/module.cc
    autoli/mutex.cc
    autoli/ns/name_server_module.cc
    autoli/ns/ns_client.cc
    autoli/ns/ns_protocol.cc
    autoli/protocol.cc
    autoli/rock/rock_protocol.cc
    autoli/rock/rock_server.cc
    autoli/rock/rock_stream.cc
    autoli/scheduler.cc
    autoli/socket.cc
    autoli/stream.cc
    autoli/streams/async_socket_stream.cc
    autoli/streams/socket_stream.cc
    autoli/streams/load_balance.cc
    autoli/streams/service_discovery.cc
    autoli/streams/zlib_stream.cc
    autoli/tcp_server.cc
    autoli/timer.cc
    autoli/thread.cc
    autoli/util.cc
    autoli/util/crypto_util.cc
    autoli/util/json_util.cc
    autoli/util/hash_util.cc
    autoli/worker.cc
    autoli/application.cc
    autoli/zk_client.cc
    )

ragelmaker(autoli/http/http11_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/autoli/http)
ragelmaker(autoli/http/httpclient_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/autoli/http)
ragelmaker(autoli/uri.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/autoli)

protobufmaker(autoli/ns/ns_protobuf.proto LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR})
#PROTOBUF_GENERATE_CPP(PB_SRCS PB_HDRS autoli/ns/ns_protobuf.proto)
#message("****${PB_SRCS}***${PB_HDRS}****")
#list(APPEND LIB_SRC ${PB_SRCS})
#
#message(STATUS ${LIB_SRC})

add_library(autoli SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(autoli)
#add_library(autoli_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (autoli_static PROPERTIES OUTPUT_NAME "autoli")

#find_library(YAMLCPP yaml-cpp)
find_library(PTHREAD pthread)
#set(LIBS ${LIBS} autoli)
#set(LIBS ${LIBS} dl)
#set(LIBS ${LIBS} ${YAMLCPP})
#set(LIBS ${LIBS} ${PTHREAD})

set(LIBS
        autoli
        dl
        pthread
        yaml-cpp
        jsoncpp
        ${ZLIB_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${PROTOBUF_LIBRARIES}
        event
        hiredis_vip
        mysqlclient
        zookeeper_mt
        sqlite3
        tinyxml2
        jemalloc
        )
autoli_add_executable(test_util "tests/test_util.cc" autoli "${LIBS}")
autoli_add_executable(test_hashmultimap "tests/test_hashmultimap.cc" autoli "${LIBS}")
autoli_add_executable(test_hashmap "tests/test_hashmap.cc" autoli "${LIBS}")
autoli_add_executable(test_dict "tests/test_dict.cc" autoli "${LIBS}")
autoli_add_executable(test_array "tests/test_array.cc" autoli "${LIBS}")
if(BUILD_TEST)
autoli_add_executable(test1 "tests/test.cc" autoli "${LIBS}")
autoli_add_executable(test_config "tests/test_config.cc" autoli "${LIBS}")
autoli_add_executable(test_thread "tests/test_thread.cc" autoli "${LIBS}")
autoli_add_executable(test_fiber "tests/test_fiber.cc" autoli "${LIBS}")
autoli_add_executable(test_scheduler "tests/test_scheduler.cc" autoli "${LIBS}")
autoli_add_executable(test_iomanager "tests/test_iomanager.cc" autoli "${LIBS}")
autoli_add_executable(test_hook "tests/test_hook.cc" autoli "${LIBS}")
autoli_add_executable(test_address "tests/test_address.cc" autoli "${LIBS}")
autoli_add_executable(test_socket "tests/test_socket.cc" autoli "${LIBS}")
autoli_add_executable(test_bytearray "tests/test_bytearray.cc" autoli "${LIBS}")
autoli_add_executable(test_http "tests/test_http.cc" autoli "${LIBS}")
autoli_add_executable(test_http_parser "tests/test_http_parser.cc" autoli "${LIBS}")
autoli_add_executable(test_tcp_server "tests/test_tcp_server.cc" autoli "${LIBS}")
autoli_add_executable(echo_server "examples/echo_server.cc" autoli "${LIBS}")
autoli_add_executable(test_http_server "tests/test_http_server.cc" autoli "${LIBS}")
autoli_add_executable(test_uri "tests/test_uri.cc" autoli "${LIBS}")
autoli_add_executable(my_http_server "samples/my_http_server.cc" autoli "${LIBS}")

autoli_add_executable(echo_server_udp "examples/echo_server_udp.cc" autoli "${LIBS}")
autoli_add_executable(echo_udp_client "examples/echo_udp_client.cc" autoli "${LIBS}")

autoli_add_executable(test_daemon "tests/test_daemon.cc" autoli "${LIBS}")
autoli_add_executable(test_env "tests/test_env.cc" autoli "${LIBS}")
autoli_add_executable(test_ws_server "tests/test_ws_server.cc" autoli "${LIBS}")
autoli_add_executable(test_ws_client "tests/test_ws_client.cc" autoli "${LIBS}")
autoli_add_executable(test_application "tests/test_application.cc" autoli "${LIBS}")

autoli_add_executable(test_http_connection "tests/test_http_connection.cc" autoli "${LIBS}")
autoli_add_executable(test_lru "tests/test_lru.cc" autoli "${LIBS}")
autoli_add_executable(test_timed_cache "tests/test_timed_cache.cc" autoli "${LIBS}")
autoli_add_executable(test_timed_lru_cache "tests/test_timed_lru_cache.cc" autoli "${LIBS}")
autoli_add_executable(test_zlib_stream "tests/test_zlib_stream.cc" autoli "${LIBS}")

endif()
autoli_add_executable(test_crypto "tests/test_crypto.cc" autoli "${LIBS}")
autoli_add_executable(test_sqlite3 "tests/test_sqlite3.cc" autoli "${LIBS}")
autoli_add_executable(test_rock "tests/test_rock.cc" autoli "${LIBS}")
autoli_add_executable(test_email  "tests/test_email.cc" autoli "${LIBS}")
autoli_add_executable(test_mysql "tests/test_mysql.cc" autoli "${LIBS}")
autoli_add_executable(test_nameserver "tests/test_nameserver.cc" autoli "${LIBS}")
autoli_add_executable(test_bitmap "tests/test_bitmap.cc" autoli "${LIBS}")
autoli_add_executable(test_zkclient "tests/test_zookeeper.cc" autoli "${LIBS}")
autoli_add_executable(test_service_discovery "tests/test_service_discovery.cc" autoli "${LIBS}")

set(ORM_SRCS
    autoli/orm/table.cc
    autoli/orm/column.cc
    autoli/orm/index.cc
    autoli/orm/orm.cc
    autoli/orm/util.cc
    )
autoli_add_executable(orm "${ORM_SRCS}" autoli "${LIBS}")

add_library(test_module SHARED tests/test_module.cc)

autoli_add_executable(bin_autoli "autoli/main.cc" autoli "${LIBS}")
set_target_properties(bin_autoli PROPERTIES OUTPUT_NAME "autoli")

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/orm_out)
#set(OLIBS ${LIBS} orm_data)
#autoli_add_executable(test_orm "tests/test_orm.cc" orm_data "${OLIBS}")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
