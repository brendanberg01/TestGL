cmake_minimum_required(VERSION 3.12)
project(TestGL)

set(CMAKE_CXX_STANDARD 17)

add_executable(TestGL main.cpp)

find_package(glfw3 3.2 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    SET(EXTRA_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif (APPLE)

target_link_libraries(TestGL
    glfw
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${EXTRA_LIBS}
)
