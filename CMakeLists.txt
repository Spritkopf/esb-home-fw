cmake_minimum_required(VERSION 3.28)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(esb-home-fw C)
endif()

add_subdirectory(common)
add_subdirectory(binary-sensor)
