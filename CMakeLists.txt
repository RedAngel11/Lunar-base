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
add_executable(Lunar_base main.cpp
        src/core/Game.cpp
        src/core/Game.h
        src/ui/TopPanel.cpp
        src/ui/TopPanel.h
        src/ui/Button.h
        src/ui/Button.cpp
        src/screens/IScreen.cpp
        src/screens/IScreen.h
        src/screens/Locationscreen.cpp
        src/screens/Locationscreen.h
        src/screens/StructureScreen.cpp
        src/screens/StructureScreen.h
        src/screens/MainMenuScreen.cpp
        src/screens/MainMenuScreen.h
        src/screens/MaterialScreen.cpp
        src/screens/MaterialScreen.h)

# Линкуем библиотеки SFML
target_link_libraries(Lunar_base
        sfml-graphics
        sfml-window
        sfml-system
)
