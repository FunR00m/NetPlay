cmake_minimum_required(VERSION 3.16)
project(Client)

set(CMAKE_CXX_STANDARD 20)

# Добавляем папку include в пути поиска заголовочных файлов
include_directories(include)

# Добавляем исходные файлы проекта
add_executable(Client
        main.cpp
        src/GameManager.cpp
        src/Object.cpp
        src/Fields.cpp
        src/PackedData.cpp
        src/utils/debug.cpp
        src/sys/InetNetworker.cpp
)

# Добавляем необходимые библиотеки, если нужно
target_link_libraries(Client thr)

# Указываем директорию для собранных бинарных файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


