cmake_minimum_required(VERSION 3.9)
project(C4-lolwuz)

set(CMAKE_CXX_STANDARD 11)

add_executable(c4-lolwuz main.cpp c4.cpp c4bot.cpp solver.cpp solver.h transposition.cpp transposition.h)