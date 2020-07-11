cmake_minimum_required(VERSION 3.7.2)

# Set binary name
set(BINARY ${CMAKE_PROJECT_NAME})

# Set source folder
set(SOURCE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

# Set source files
set(FP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FPGrowth/FPGrowth.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DataComponent/DataComponent.cpp)

# Set header files
set(FP_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/FPGrowth/FPGrowth.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DataComponent/DataComponent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DataComponent/CSV.h)

# Create executable BINARY
add_executable(${BINARY}_run ${FP_HEADERS} ${FP_SOURCES})

if(UNIX)
    target_link_libraries(${BINARY}_run pthread)
endif(UNIX)

# Create a static library to be used in /test
add_library(${BINARY}_lib STATIC ${FP_HEADERS} ${FP_SOURCES})
