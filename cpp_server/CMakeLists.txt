cmake_minimum_required(VERSION 3.10)
project(cpp_server)

# Используем стандарт C++17
set(CMAKE_CXX_STANDARD 17)

# Ищем библиотеку libpq (PostgreSQL C API)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQ REQUIRED libpq)

# Создаем исполняемый файл
add_executable(cpp_server main.cpp)

# Подключаем заголовочные файлы и библиотеки
target_include_directories(cpp_server PRIVATE ${LIBPQ_INCLUDE_DIRS})
target_link_libraries(cpp_server PRIVATE ${LIBPQ_LIBRARIES})