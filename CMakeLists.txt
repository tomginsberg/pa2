cmake_minimum_required(VERSION 3.6)
project(ITP)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

link_directories(/usr/local/pgsql/lib/)

message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message("C Flags = ${CMAKE_CXX_FLAGS_DEBUG}")

add_executable(itp testFills.cpp)

add_custom_command(
        TARGET itp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/config.ini
        ${CMAKE_CURRENT_BINARY_DIR})