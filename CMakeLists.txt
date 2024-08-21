cmake_minimum_required(VERSION 3.25)
project(vsg_viewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(dependencies/VulkanSceneGraph)
add_subdirectory(dependencies/vsgXchange)
add_subdirectory(dependencies/vsgImGui)

add_executable(vsg_viewer src/main.cpp
    include/Application.h
    include/GUI.h
    include/RouteLoader.h

    src/Application.cpp
    src/GUI.cpp
    src/RouteLoader.cpp
)

target_include_directories(vsg_viewer PRIVATE include)
target_link_libraries(vsg_viewer PRIVATE vsg vsgXchange vsgImGui)
