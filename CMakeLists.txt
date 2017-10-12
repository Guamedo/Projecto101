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

add_executable(Project101   Project101.cpp
        World.h
                            World.cpp
        Box.cpp
                            Box.h
        Vector2.cpp
                            Vector2.h
                            EntityPrime.cpp
                            EntityPrime.h
                            CachoPrime.cpp
                            CachoPrime.h Utiles.cpp Utiles.h)
                            
target_link_libraries(Project101 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})