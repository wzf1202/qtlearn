cmake_minimum_required(VERSION 3.16)

project(qtlearn VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TASK1 "Build Qt task1" OFF)
option(BUILD_TASK2 "Build Win32 task2" OFF)
option(BUILD_TASK3 "Build QT task3" ON)
option(BUILD_THREADLEARN "Build threadlearn" OFF)


if(BUILD_TASK1)
    add_subdirectory(task1)
endif()

if(BUILD_TASK2)
    add_subdirectory(task2)
endif()

if(BUILD_TASK3)
    add_subdirectory(task3)
endif()

if(BUILD_THREADLEARN)
    add_subdirectory(threadlearn)
endif()