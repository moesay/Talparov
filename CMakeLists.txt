cmake_minimum_required(VERSION 3.17)
project(Talparov)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(GCC_COVERGAE_COMPILE_FLAGS "-Wno-enum-compare -O3")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${GCC_COVERGAE_COMPILE_FLAGS})

add_executable(Talparov source/main.cpp include/defs.h source/board.cpp
    include/board.h include/utils.h source/utils.cpp source/consts.cpp
    include/consts.h include/bitboard.h source/bitboard.cpp include/move.h
    source/move.cpp include/movegen.h source/movegen.cpp include/tptable.h
    source/tptable.cpp include/misc.h source/misc.cpp include/search.h
    source/search.cpp include/talparov.h source/talparov.cpp)