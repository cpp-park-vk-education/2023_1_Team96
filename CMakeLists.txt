cmake_minimum_required(VERSION 3.0.0)
project(Tactics VERSION 1.0.1)

find_package(SFML 2.5 COMPONENTS window graphics REQUIRED)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(QT_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/qt)
qt_wrap_cpp(Tactics_H ${QT_HEADER_DIR}/chooseopponentform.h ${QT_HEADER_DIR}/entranceform.h ${QT_HEADER_DIR}/gameform.h ${QT_HEADER_DIR}/mainwindow.h)

set(QT_UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(QT_UI_COMPILED ${CMAKE_CURRENT_BINARY_DIR}/ui)
execute_process(COMMAND mkdir ${QT_UI_COMPILED})

list(APPEND QT_WINDOWS "entranceform" "chooseopponentform" "gameform" "mainwindow")
foreach(QT_UI IN LISTS QT_WINDOWS)
    set(QT_UI_OUT_FILE "${QT_UI_COMPILED}/ui_${QT_UI}.h")
    set(QT_UIS_FULL_NAME_WITH_EXT "${QT_UI_DIR}/${QT_UI}.ui")
    execute_process(COMMAND uic -o ${QT_UI_OUT_FILE} ${QT_UIS_FULL_NAME_WITH_EXT})
endforeach()

set(QT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/qt)
# set(Tactics_SRC main.cpp ${QT_SRC_DIR}/chooseopponentform.cpp ${QT_SRC_DIR}/entranceform.cpp ${QT_SRC_DIR}/gameform.cpp ${QT_SRC_DIR}/mainwindow.cpp)
file(GLOB MODEL "src/model/*.cpp")
file(GLOB GRAPHICS "src/graphics/*.cpp")
file(GLOB INPUT "src/input/*.cpp")
file(GLOB SERVER "src/server/*.cpp")
set(Tactics_SRC main.cpp ${QT_SRC_DIR}/chooseopponentform.cpp ${QT_SRC_DIR}/entranceform.cpp ${QT_SRC_DIR}/gameform.cpp ${QT_SRC_DIR}/mainwindow.cpp ${GRAPHICS} ${MODEL} ${INPUT} ${SERVER} )

add_executable(tactics ${Tactics_SRC} ${Tactics_H})
# add_executable(tactics ${Tactics_SRC})LIC ${QT_UI_COMPILED})
target_link_libraries(tactics Qt5::Widgets)

target_include_directories(tactics PUBLIC include/qt)
target_include_directories(tactics PUBLIC ${QT_UI_COMPILED})
target_link_libraries(tactics Qt5::Widgets)

# find_package(Boost REQUIRED COMPONENTS system thread date_time regex serialization)
# target_link_libraries(tactics Boost::system Boost::thread Boost::date_time Boost::regex Boost::serialization)
target_link_libraries(tactics sfml-window sfml-graphics sfml-network)
target_link_libraries(server sfml-network)

include_directories(include)
