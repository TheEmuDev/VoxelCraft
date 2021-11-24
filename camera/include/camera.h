#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<cglm/cglm.h>

#include "shaderClass.h"

class Camera
{
    public:
        vec3 Position;
        vec3 Orientation = {0.0f, 0.0f, -1.0f};
        vec3 Up = {0.0f, 1.0f, 0.0f};

        bool firstClick = true;

        int width;
        int height;

        float speed = 0.1f;
        float sensitivity = 100.0f;

        Camera(int width, int height, vec3 position);

        void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
        void Inputs(GLFWwindow* window);
};

#endif