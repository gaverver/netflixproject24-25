cmake_minimum_required(VERSION 3.10)

project(NetflixRecommendTests)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)   

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(runRecommendTests testsForRecommend.cpp ../../recommendCommand.cpp ../../DBFile.cpp 
../../ConsoleMenu.cpp )

# Specify where to find the header files
target_include_directories(runRecommendTests PRIVATE
    ../../
)

target_link_libraries(runRecommendTests gtest_main)