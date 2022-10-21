add_definitions(
    -DTW_STATIC
    -DTW_NO_LIB_PRAGMA
    -DTW_NO_DIRECT3D
    -DGLEW_STATIC
    -D_CRT_SECURE_NO_WARNINGS
)

# Directories
add_subdirectory(glfw-3.3.8)

# Include Directories
include_directories(
    glew-2.1.0/include
    glfw-3.3.8/include/GLFW
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(OPENGL_LIBRARY
        ${OPENGL_LIBRARY}
        -lGL -lGLU -lXrandr -lXext -lX11 -lrt
        ${CMAKE_DL_LIBS}
        ${GLFW_LIBRARIES}
    )
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(OPENGL_LIBRARY
        ${OPENGL_LIBRARY}
        ${CMAKE_DL_LIBS}
        ${GLFW_LIBRARIES}
    )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

set(GLEW_SOURCE
    glew-2.1.0/src/glew.c
)

set(GLEW_HEADERS)

add_library( GLEW2 STATIC 
            ${GLEW_SOURCE} 
            ${GLEW_INCLUDE}
)
target_link_libraries(GLEW2 ${OPENGL_LIBRARY} ${EXTRA_LIBS})