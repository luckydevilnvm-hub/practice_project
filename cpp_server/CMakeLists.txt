
cmake_minimum_required(VERSION 3.10)
project(cpp_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Явно указываем пути
set(PG_INCLUDE_DIR /usr/include/postgresql)
set(PG_LIBRARY /usr/lib/x86_64-linux-gnu/libpq.so)

# Crow
find_path(CROW_INCLUDE_DIR crow.h PATH_SUFFIXES include)
include_directories(${CROW_INCLUDE_DIR} ${PG_INCLUDE_DIR})

add_executable(server server.cpp)
target_link_libraries(server ${PG_LIBRARY} pthread)
target_include_directories(server PRIVATE ${PG_INCLUDE_DIR})
