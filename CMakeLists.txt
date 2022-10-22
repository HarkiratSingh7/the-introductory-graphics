cmake_minimum_required(VERSION 3.0)

project(The-Introductory-Graphics)

find_package(OpenGL REQUIRED)

add_subdirectory(Dependencies)

include_directories(
    Dependencies/glfw-3.3.8/include/
    Dependencies/glew-2.1.0/include/
    include/
    .
)

set(REQUIRED_LIBS ${OPENGL_LIBRARY} glfw GLEW2)

add_definitions(
    -DTW_STATIC
    -DTW_NO_LIB_PRAGMA
    -DTW_NO_DIRECT3D
    -DGLEW_STATIC
    -D_CRT_SECURE_NO_WARNINGS
)

# DDA Algorithm Demonstration
add_executable(DDADemo src/DDADemo/main.cpp)
target_link_libraries(DDADemo ${REQUIRED_LIBS})

# Mid Point Circle Demostration
add_executable(MidPtCircleDemo src/MidPointCircleDemo/main.cpp)
target_link_libraries(MidPtCircleDemo ${REQUIRED_LIBS})

# Mid Point Ellipse Demonstration
add_executable(MidPtEllipseDemo src/MidPointEllipseDemo/main.cpp)
target_link_libraries(MidPtEllipseDemo ${REQUIRED_LIBS})