cmake_minimum_required(VERSION 3.10.2)
project(roguelike_game_svevash)

set(CMAKE_CXX_STANDARD 17)

set(src main.cpp game.hpp game.cpp characters.hpp characters.cpp)

add_executable(roguelike_game_svevash ${src})

add_compile_options("-fsanitize=address -g")

target_link_libraries(${PROJECT_NAME} curses)