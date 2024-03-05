cmake_minimum_required(VERSION 3.23)
project(Client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)

# Добавляем исходные файлы проекта
set(COMPILE_FILES
        src/GameManager.cpp
        src/Object.cpp
        src/Fields.cpp
        src/PackedData.cpp
        src/utils/debug.cpp
        src/sys/InetNetworker.cpp
        src/Components/Transform.cpp
        src/Components/Sprite.cpp
        src/Components/AnimatedSprite.cpp
        src/Systems/RenderSystem.cpp
        src/sys/TextureSDL.cpp
        src/sys/RendererSDL.cpp
        src/ComponentManager.cpp
        src/Controller.cpp
        src/Keyboard.cpp
)

add_library(Client
        ${COMPILE_FILES}
)

add_executable(EngineTest
        main.cpp
        ${COMPILE_FILES}
)

# Добавляем папку include в пути поиска заголовочных файлов
target_include_directories(Client PRIVATE include)
target_include_directories(EngineTest PRIVATE include)
target_include_directories(Client PUBLIC third-party/include)
target_include_directories(EngineTest PUBLIC third-party/include)

target_link_directories(Client PUBLIC third-party/lib)
target_link_directories(EngineTest PUBLIC third-party/lib)
target_link_libraries(Client SDL2 SDL2_image)
target_link_libraries(EngineTest SDL2 SDL2_image)

# Указываем директорию для собранных бинарных файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


