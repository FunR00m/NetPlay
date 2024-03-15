cmake_minimum_required(VERSION 3.16)
project(Server)

set(CMAKE_CXX_STANDARD 20)

# Добавляем папку include в пути поиска заголовочных файлов
include_directories(include)

# Добавляем исходные файлы проекта
set(COMPILE_FILES
        src/Fields.cpp
        src/GameManager.cpp
        src/Object.cpp
        src/PackedData.cpp
        src/utils/debug.cpp
        src/sys/InetNetworker.cpp
        src/Components/Transform.cpp
        src/Components/Sprite.cpp
        src/Components/Collider.cpp
        src/Components/TriggerCollider.cpp
        src/Systems/MoveSystem.cpp
        src/Controller.cpp
)

add_executable(EngineTestServer
    main.cpp
    ${COMPILE_FILES}
)

add_library(Server
    ${COMPILE_FILES}
)


# Добавляем необходимые библиотеки, если нужно
# if(CMAKE_SYSTEM_NAME EQUAL "FreeBSD")
#        target_link_libraries(Server thr)
# endif()

# Указываем директорию для собранных бинарных файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


