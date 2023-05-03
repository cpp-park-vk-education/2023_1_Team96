cmake_minimum_required(VERSION 3.0.0)
project(Tactics VERSION 1.0.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tactics main.cpp src/chooseopponentform.cpp src/entranceform.cpp src/gameform.cpp src/mainwindow.cpp
src/chooseopponentform.ui src/entranceform.ui src/gameform.ui src/mainwindow.ui)
target_include_directories(tactics PUBLIC include)
find_package(SFML 2.5 COMPONENTS window graphics REQUIRED)
target_link_libraries(tactics sfml-window sfml-graphics)

find_package(Qt5 COMPONENTS Widgets REQUIRED)


qt5_use_modules(tactics Widgets)

add_subdirectory(lib/eventpp)
target_link_libraries(tactics eventpp)

target_link_libraries(tactics Qt5::Widgets)

# add_subdirectory(lib/qt-widgets)
# target_link_libraries(tactics qt-widgets)



