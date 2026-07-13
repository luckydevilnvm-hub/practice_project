FROM ubuntu:22.04

# Устанавливаем компилятор, cmake, библиотеки для PostgreSQL и БИБЛИОТЕКУ ASIO
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    pkg-config \
    libpq-dev \
    libasio-dev \
    curl \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Скачиваем фреймворк Crow
RUN curl -sL https://github.com/CrowCpp/Crow/releases/download/v1.2.0/crow_all.h -o crow_all.h

# Копируем файлы проекта
COPY CMakeLists.txt .
COPY main.cpp .

# Собираем проект
RUN cmake . && make

# Запускаем скомпилированный сервер
CMD ["./cpp_server"]