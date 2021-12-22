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
        vec3 Orientation = {0.0f, 0.0f, -1.0f};         // Front facing direction
        vec3 CameraUp = {0.0f, 1.0f, 0.0f};                   // Up direction
        vec3 WorldUp = {0.0f, 1.0f, 0.0f};
        vec3 Right = {1.0f, 0.0f, 0.0f};
        
        mat4 cameraMatrix = GLM_MAT4_IDENTITY_INIT;

        bool firstClick = true;

        int width;
        int height;

        float speed;
        float sensitivity;
        float zoom;

        float yaw;
        float pitch;


        Camera(int width, int height, vec3 position);

        void UpdateMatrix(float nearPlane, float farPlane);
        void Matrix(Shader& shader, const char* uniform);
        void ProcessKeyboardInputs(GLFWwindow* window, float deltaTime);
        void ProcessMouseMovement(float xOffset, float yOffset);
        void ProcessMouseScroll(float yOffset);

    private:
        const float YAW = -90.0f;
        const float PITCH = 0.0f;
        const float SPEED = 0.1f;
        const float SENSITIVITY = 0.75f;
        const float ZOOM = 45.0f;
};

#endif