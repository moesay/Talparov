cmake_minimum_required(VERSION 3.16)
project(Talparov)

file(GLOB project_files
    "./include/*.h"
    "./source/*.cpp"
    )

set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 20)

add_executable(Talparov ${project_files})
