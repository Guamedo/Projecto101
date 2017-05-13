cmake_minimum_required(VERSION 3.7)
PROJECT(Project101)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -fpermissive")

add_executable(Project101   Project101.cpp
                            Action.h
                            Action.cpp
                            Animacion.h
                            Animacion.cpp
                            Dot.h
                            Dot.cpp
                            Frame.h
                            Frame.cpp
                            World.h
                            World.cpp Cacho.cpp Cacho.h)

target_link_libraries(Project101 glut32.lib -lopengl32 -lglu32)