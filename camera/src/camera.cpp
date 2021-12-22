#include "camera.h"

Camera::Camera(int width, int height, vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    glm_vec3_dup(position, Position);

    speed = SPEED;
    sensitivity = SENSITIVITY;
    zoom = ZOOM;
    yaw = YAW;
    pitch = PITCH;
}

void Camera::UpdateMatrix(float nearPlane, float farPlane)
{
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    mat4 newCamMatrix = GLM_MAT4_IDENTITY_INIT;

    vec3 temp;
    glm_vec3_add(Position, Orientation, temp);
    glm_lookat(Position, temp, CameraUp, view);
    glm_perspective(glm_rad(zoom), (float)width / height, nearPlane, farPlane, projection);
    glm_mat4_mul(projection, view, newCamMatrix);
    glm_mat4_dup(newCamMatrix, cameraMatrix);
}

void Camera::Matrix(Shader &shader, const char *uniform)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, cameraMatrix[0]);
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset)
{
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    vec3 newOrientation;
    newOrientation[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    newOrientation[1] = sin(glm_rad(pitch));
    newOrientation[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    glm_normalize_to(newOrientation, Orientation);

    glm_vec3_cross(Orientation, WorldUp, Right);
    glm_normalize(Right);

    glm_vec3_cross(Right, Orientation, CameraUp);
    glm_normalize(CameraUp);
}

void Camera::ProcessKeyboardInputs(GLFWwindow *window, float deltaTime)
{


    // move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        vec3 frontVelocity;
        glm_vec3_scale(Orientation, speed, frontVelocity);
        glm_vec3_add(Position, frontVelocity, Position);
    }

    // move left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {

        glm_vec3_cross(Orientation, WorldUp, Right);
        glm_vec3_normalize(Right);
        glm_vec3_negate(Right);
        glm_vec3_scale(Right, speed, Right);
        glm_vec3_add(Position, Right, Position);
    }

    // move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        vec3 Temp;
        glm_vec3_negate_to(Orientation, Temp);
        glm_vec3_scale(Temp, speed, Temp);
        glm_vec3_add(Position, Temp, Position);
    }

    // move right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        vec3 Cross;
        glm_vec3_cross(Orientation, CameraUp, Cross);
        glm_vec3_normalize(Cross);
        glm_vec3_scale(Cross, speed, Cross);
        glm_vec3_add(Position, Cross, Position);
    }

    // move up (camera perspective)
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        vec3 ScaledVec;
        glm_vec3_scale(CameraUp, speed, ScaledVec);
        glm_vec3_add(Position, ScaledVec, Position);
    }

    // move down (camera perspective)
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        vec3 ScaledVec;
        glm_vec3_scale(CameraUp, speed, ScaledVec);
        glm_vec3_negate(ScaledVec);
        glm_vec3_add(Position, ScaledVec, Position);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed = 4 * SPEED;
    }

    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        speed = SPEED;
    }
}