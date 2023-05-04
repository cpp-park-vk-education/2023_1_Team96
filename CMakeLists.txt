cmake_minimum_required(VERSION 3.0.0)
project(Tactics VERSION 1.0.1)

add_executable(tactics main.cpp)

find_package(SFML 2.5 COMPONENTS window graphics REQUIRED)
target_link_libraries(tactics sfml-window sfml-graphics)

include_directories(include)
