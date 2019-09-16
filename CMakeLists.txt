cmake_minimum_required(VERSION 3.7)

set(CMAKE_BUILD_TYPE Debug)
add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(SDL2Test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} src)
add_executable(SnakeGame src/main.cpp src/game.cpp src/renderer.cpp src/snake.cpp src/input_dispatcher.cpp)
target_link_libraries(SnakeGame ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY})
