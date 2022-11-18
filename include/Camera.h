#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "core/Core.h"
#include "gfx/Shader.h"

class Camera
{
    public:
        glm::vec3 Position;
        glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);         // Front facing direction
        glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);                   // Up direction
        glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
        
        glm::mat4 cameraMatrix = glm::mat4(1.0f);

        bool firstClick = true;

        int width;
        int height;

        float speed;
        float sensitivity;
        float zoom;

        float yaw;
        float pitch;


        Camera(int width, int height, glm::vec3 position);

        void UpdateMatrix(float nearPlane, float farPlane);
        void Matrix(Shader& shader, const char* uniform);
        void ProcessKeyboardInputs(GLFWwindow* window, double deltaTime);
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