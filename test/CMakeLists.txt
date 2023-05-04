cmake_minimum_required(VERSION 3.5)
project(test)
add_executable(test main.cpp)
add_executable(server server.cpp)
add_executable(temp temp.cpp)
find_package(Boost REQUIRED COMPONENTS system thread date_time regex serialization)

target_link_libraries(test Boost::system Boost::thread Boost::date_time Boost::regex Boost::serialization)