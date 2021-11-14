cmake_minimum_required(VERSION 3.22.0)

project(VoxelCraft VERSION 0.0.1)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(external/glfw)

target_include_directories(${PROJECT_NAME} 
    PUBLIC external/glfw/include
)

target_link_directories(${PROJECT_NAME}
    PUBLIC external/glfw/src
)

target_link_libraries(${PROJECT_NAME} glfw)