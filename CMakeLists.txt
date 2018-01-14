cmake_minimum_required(VERSION 3.7)
PROJECT(Project101)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if (NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif (NOT GLUT_FOUND)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if (NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif (NOT OPENGL_FOUND)

add_executable(Project101 Project101.cpp
                          Box.cpp
                          Box.h
                          Level.cpp
                          Level.h
                          Agent.cpp
                          Agent.h
                          Player.cpp
                          Player.h
                          MainGame.cpp
                          MainGame.h
                          Enemy.cpp
                          Enemy.h
                          PerlinNoise.cpp
                          PerlinNoise.h Camera.cpp Camera.h)
                            
target_link_libraries(Project101 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})