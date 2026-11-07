cmake_minimum_required(VERSION 3.16)
project(Task2_Peresechenie LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_GUI "Build Qt interface" ON)
add_library(geometry STATIC src/geometry/Geometry.cpp src/geometry/Geometry.h)
target_include_directories(geometry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(BUILD_GUI)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    set(CMAKE_AUTOMOC ON)
    add_library(geometry_qt STATIC src/geometry_qt/GeometryQt.cpp src/geometry_qt/GeometryQt.h)
    target_include_directories(geometry_qt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(geometry_qt PUBLIC geometry Qt6::Widgets)

    add_executable(Task2 src/main.cpp src/Canvas.h src/Canvas.cpp)
    target_link_libraries(Task2 PRIVATE geometry geometry_qt Qt6::Widgets)
endif()

enable_testing()
add_executable(test_geometry tests/test_geometry.cpp)
target_link_libraries(test_geometry PRIVATE geometry)
add_test(NAME task2.geometry COMMAND test_geometry)
