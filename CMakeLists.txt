cmake_minimum_required(VERSION 3.7.2)

if(WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif(WIN32)

# Name of the project
project(FrequentPatternGrowth)

message(STATUS "${PROJECT_NAME} is located in ${CMAKE_CURRENT_SOURCE_DIR}")

# Specify C++ version
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directory for source code
include_directories(src)

# Include CMake files
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(lib/googletest)

if(VVPS_DEBUG)
    message(STATUS "VVPS_DEBUG Enabled")
endif(VVPS_DEBUG)

if(CMAKE_BUILD_TYPE EQUAL "RELEASE")
    message(STATUS "Release Mode")
endif(CMAKE_BUILD_TYPE EQUAL "RELEASE")
