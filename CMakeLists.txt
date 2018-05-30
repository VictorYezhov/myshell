cmake_minimum_required(VERSION 3.5)
project(myshell)


set(CMAKE_CXX_STANDARD 14)

add_executable(myshell main.cpp Source/Command.cpp
        headers/Command.h Source/Merrno.cpp headers/Merrno.h
        Source/Mpwd.cpp headers/Mpwd.h Source/Mcd.cpp headers/Mcd.h
        Source/Mexit.cpp headers/Mexit.h headers/ErrorInfo.h
        Source/ErrorInfo.cpp Source/IOStreams.cpp headers/IOStreams.h
        Source/Desider.cpp headers/Desider.h)