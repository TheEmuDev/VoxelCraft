cmake_minimum_required(VERSION 3.22.0)

project(VoxelCraft VERSION 0.0.1)

add_executable(${PROJECT_NAME} main.cpp 
    external/glad/src/glad.c
    shaders/src/shaderClass.cpp
    buffers/src/EBO.cpp
    buffers/src/VAO.cpp
    buffers/src/VBO.cpp
)

add_subdirectory(external/glfw)

target_include_directories(${PROJECT_NAME} 
    PUBLIC external/glfw/include
    PUBLIC external/glad/include
    PUBLIC shaders/include
    PUBLIC buffers/include
)

target_link_directories(${PROJECT_NAME}
    PUBLIC external/glfw/src
    PUBLIC external/glad/src
    PUBLIC shaders/src
    PUBLIC buffers/src
)

target_link_libraries(${PROJECT_NAME} glfw m)