cmake_minimum_required(VERSION 3.5)
project(tactics)


add_subdirectory(lib/server)

add_executable(tactics src/main.cpp)
target_link_libraries(tactics server)

find_package(Boost REQUIRED COMPONENTS system thread date_time regex serialization)

target_link_libraries(tactics Boost::system Boost::thread Boost::date_time Boost::regex Boost::serialization)