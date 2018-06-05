cmake_minimum_required(VERSION 3.1)
# プロジェクト名と使用する言語を設定
# Using c++14 for all personal projects
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Where to build the files

#set(CMAKE_CXX_FLAGS "-Wall -O0 -g")
set(CMAKE_CXX_FLAGS "-Wall -O2")

# Change project name here
set(PROJECT_NAME hexagon)
project(${PROJECT_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


include_directories(include)

add_subdirectory(tests)
