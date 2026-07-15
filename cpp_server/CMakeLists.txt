cmake_minimum_required(VERSION 3.10)
project(cpp_server)

# Используем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ПРОВЕРКА НЕОБХОДИМЫХ БИБЛИОТЕК
# Проверяем наличие pkg-config (нужен для поиска libpq)
find_package(PkgConfig QUIET)
if(NOT PkgConfig_FOUND)
    message(FATAL_ERROR 
        "ОШИБКА: Не найден pkg-config!\n"
        "Для работы проекта необходимо установить pkg-config.\n"
        "В Docker это делается автоматически через Dockerfile.\n"
        "Для локальной установки в Ubuntu выполните:\n"
        "  sudo apt install pkg-config\n"
    )
endif()

# Проверяем наличие библиотеки PostgreSQL (libpq)
pkg_check_modules(LIBPQ QUIET libpq)
if(NOT LIBPQ_FOUND)
    message(FATAL_ERROR 
        "ОШИБКА: Не найдена библиотека libpq (PostgreSQL)!\n"
        "Для работы проекта необходимо установить libpq-dev.\n"
        "В Docker это делается автоматически через Dockerfile.\n"
        "Для локальной установки в Ubuntu выполните:\n"
        "  sudo apt install libpq-dev\n"
        "Или скачайте PostgreSQL с официального сайта:\n"
        "  https://www.postgresql.org/download/\n"
    )
endif()

# Проверяем наличие библиотеки Asio (нужна для Crow)
pkg_check_modules(ASIO QUIET asio)
if(NOT ASIO_FOUND)
    # Asio может быть установлен как libasio-dev
    find_path(ASIO_INCLUDE_DIR asio.hpp PATHS /usr/include /usr/local/include)
    if(NOT ASIO_INCLUDE_DIR)
        message(FATAL_ERROR 
            "ОШИБКА: Не найдена библиотека Asio!\n"
            "Для работы проекта необходимо установить libasio-dev.\n"
            "В Docker это делается автоматически через Dockerfile.\n"
            "Для локальной установки в Ubuntu выполните:\n"
            "  sudo apt install libasio-dev\n"
            "Или скачайте Asio с официального сайта:\n"
            "  https://think-async.com/Asio/\n"
            )
    endif()
endif()


# СОЗДАНИЕ ИСПОЛНЯЕМОГО ФАЙЛА
add_executable(cpp_server main.cpp)


# ПОДКЛЮЧЕНИЕ БИБЛИОТЕК К ПРОЕКТУ
# Подключаем заголовочные файлы PostgreSQL
target_include_directories(cpp_server PRIVATE ${LIBPQ_INCLUDE_DIRS})

# Подключаем библиотеки PostgreSQL
target_link_libraries(cpp_server PRIVATE ${LIBPQ_LIBRARIES})

# Добавляем флаги компиляции из pkg-config
target_compile_options(cpp_server PRIVATE ${LIBPQ_CFLAGS_OTHER})

# Если Asio найден через pkg-config, подключаем его
if(ASIO_FOUND)
    target_include_directories(cpp_server PRIVATE ${ASIO_INCLUDE_DIRS})
endif()

