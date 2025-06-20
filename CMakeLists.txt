cmake_minimum_required(VERSION 3.10)

project(MyExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

#For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Glob all .cpp and .h files in the src directory
file(GLOB SOURCES_AND_HEADERS
    ${CMAKE_SOURCE_DIR}/src/recommendation_system/*.cpp
    ${CMAKE_SOURCE_DIR}/src/recommendation_system/*.h
)

# Explicitly remove app.cpp and app.h
list(REMOVE_ITEM SOURCES_AND_HEADERS
    "${CMAKE_SOURCE_DIR}/src/recommendation_system/main.cpp"
) 

file(GLOB TESTS 
    ${CMAKE_SOURCE_DIR}/src/recommendation_system/tests/*/*.cpp
    ${CMAKE_SOURCE_DIR}/src/recommendation_system/tests/*/*.h    
)

#Correct paths relative to /usr/src/myapp when CMake is run from the build directory
add_executable(runTests ${SOURCES_AND_HEADERS} ${TESTS})

target_link_libraries(runTests gtest_main)