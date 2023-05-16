cmake_minimum_required(VERSION 3.25)
project(BasicGameRawCPP)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(BasicGameRawCPP
        Actor.cpp
        Actor.h
        Component.cpp
        Component.h
        Game.cpp
        Game.h
        Main.cpp
        Paddle.cpp
        Paddle.h)
