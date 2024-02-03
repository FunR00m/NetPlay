cmake_minimum_required(VERSION 3.16)
project(Client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Добавляем папку include в пути поиска заголовочных файлов
include_directories(include)

# include_directories(/usr/local/include)
include_directories(third-party/include)

# Добавляем исходные файлы проекта
add_executable(Client
        main.cpp
        src/GameManager.cpp
        src/Object.cpp
        src/Fields.cpp
        src/PackedData.cpp
        src/utils/debug.cpp
        src/sys/InetNetworker.cpp
        src/Components/Transform.cpp
        src/Components/Sprite.cpp
        src/Systems/RenderSystem.cpp
        src/sys/TextureSDL.cpp
        src/sys/RendererSDL.cpp
        src/ComponentManager.cpp
        src/Controller.cpp
)

# Добавляем необходимые библиотеки, если нужно
# if(CMAKE_SYSTEM_NAME EQUAL "FreeBSD")
#        target_link_libraries(Client thr)
# endif()

# target_link_directories(Client PUBLIC /usr/local/lib)
target_link_directories(Client PUBLIC third-party/lib)
target_link_libraries(Client SDL2 SDL2_image)

configure_file(${CMAKE_SOURCE_DIR}/image.png ${CMAKE_BINARY_DIR}/image.png COPYONLY)

# Указываем директорию для собранных бинарных файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


