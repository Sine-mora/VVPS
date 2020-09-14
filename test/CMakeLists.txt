cmake_minimum_required(VERSION 3.7.2)

# Set binary name
set(BINARY ${CMAKE_PROJECT_NAME}_test)

# Set source files
set(FP_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FPTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VVPSTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PhilippeFourierExampleTest.cpp)

# Create executable BINARY
add_executable(${BINARY} ${FP_TEST_SOURCES})

# Add a test to the project to be run by ctest(1).
add_test(NAME ${BINARY} COMMAND ${BINARY})

# Link the source code library with the test
if(UNIX)
    target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib pthread gtest)
endif(UNIX)
if(WIN32)
    target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)
endif(WIN32)
