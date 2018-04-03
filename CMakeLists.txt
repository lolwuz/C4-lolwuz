cmake_minimum_required(VERSION 3.9)
project(C4-lolwuz)

set(CMAKE_CXX_STANDARD 11)

add_executable(c4-lolwuz main.cpp c4.cpp c4bot.cpp Solver.cpp Solver.h Transposition.cpp Transposition.h)