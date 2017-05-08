cmake_minimum_required(VERSION 2.8)
# Project Name
PROJECT(Project101)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")

add_executable(Project101 Project101.cpp
                            Action.h
                            Action.cpp
                            Animacion.h
                            Animacion.cpp
                            Dot.h
                            Dot.cpp
                            Frame.h
                            Frame.cpp
                            World.h
                            World.cpp)

target_link_libraries(Project101 glut32.lib opengl32 glu32 gdi32 comdlg32 user32 mingw32)