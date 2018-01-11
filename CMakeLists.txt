cmake_minimum_required(VERSION 3.8)
project(SignauxC)

set(CMAKE_C_STANDARD 99)
set(SOURCE_FILES main.c Djikstra.h)
add_executable(SignauxC ${SOURCE_FILES} Djikstra.h)
target_link_libraries(SignauxC m pthread)