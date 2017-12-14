cmake_minimum_required(VERSION 3.9)
project(Thread C)


set(CMAKE_C_STANDARD 99)
add_executable(Main  main.c)

TARGET_LINK_LIBRARIES(Main pthread )