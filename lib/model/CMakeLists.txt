add_library(model STATIC src/gamemodel.cpp src/gameobject.cpp)
target_include_directories(model PUBLIC include)
target_link_libraries(model command_manager)
