cmake_minimum_required(VERSION 3.20)
project(Lunar_base)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
# Указываем путь к SFML
set(SFML_DIR "${CMAKE_SOURCE_DIR}/sfml/lib/cmake/SFML")

link_directories("${CMAKE_SOURCE_DIR}/sfml/lib")
# путь к заголовочным файлам
include_directories("${CMAKE_SOURCE_DIR}/sfml/include")

#set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)

# Добавляем исполняемый файл
add_executable(Lunar_base main.cpp)

# Линкуем библиотеки SFML
target_link_libraries(Lunar_base
        sfml-graphics
        sfml-window
        sfml-system
)
