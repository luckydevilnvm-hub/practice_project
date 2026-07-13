FROM ubuntu:22.04

# Устанавливаем компилятор и зависимости
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    make \
    libpq-dev \
    libasio-dev \
    git \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Устанавливаем crow (веб-фреймворк)
WORKDIR /opt
RUN git clone https://github.com/CrowCpp/Crow.git && \
    cd Crow && \
    mkdir build && \
    cd build && \
    cmake .. -DCROW_BUILD_EXAMPLES=OFF -DCROW_BUILD_TESTS=OFF && \
    make install

# Копируем исходники
WORKDIR /app
COPY server.cpp .
COPY CMakeLists.txt .

# Компилируем
RUN mkdir build && \
    cd build && \
    cmake .. && \
    make

# Открываем порт
EXPOSE 8080

# Запускаем
CMD ["./build/server"]