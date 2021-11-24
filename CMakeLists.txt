cmake_minimum_required(VERSION 3.22.0)

project(VoxelCraft VERSION 0.0.1)

add_executable(${PROJECT_NAME} main.cpp 
    external/glad/src/glad.c
    shader/src/shaderClass.cpp
    buffers/src/EBO.cpp
    buffers/src/VAO.cpp
    buffers/src/VBO.cpp
    texture.cpp
    camera/src/camera.cpp
)

add_subdirectory(external/glfw)
add_subdirectory(external/cglm)
add_subdirectory(external/stb)

target_include_directories(${PROJECT_NAME} 
    PUBLIC external/cglm/include
    PUBLIC external/glfw/include
    PUBLIC external/glad/include
    PUBLIC shader/include
    PUBLIC buffers/include
    PUBLIC camera/include
)

target_link_directories(${PROJECT_NAME}
    PUBLIC external/cglm/src
    PUBLIC external/glfw/src
    PUBLIC external/glad/src
    PUBLIC shader/src
    PUBLIC buffers/src
    PUBLIC camera/src
)

target_link_libraries(${PROJECT_NAME} glfw m cglm stb)