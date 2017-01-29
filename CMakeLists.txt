cmake_minimum_required(VERSION 3.5)
project(morseCode)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    morseCode.ino)

add_executable(morseCode ${SOURCE_FILES})