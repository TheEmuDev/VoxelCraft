#include "camera.h"

Camera::Camera(int width, int height, vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    glm_vec3_dup(position, Position);
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    mat4 newCamMatrix = GLM_MAT4_IDENTITY_INIT;

    vec3 temp;
    glm_vec3_add(Position, Orientation, temp);
    glm_lookat(Position, temp, Up, view);
    glm_perspective(glm_rad(FOVdeg), (float)(width / height), nearPlane, farPlane, projection);
    glm_mat4_mul(projection, view, newCamMatrix);
    glm_mat4_dup(newCamMatrix, cameraMatrix);
}

void Camera::Matrix(Shader &shader, const char *uniform)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, cameraMatrix[0]);
}

void Camera::Inputs(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        vec3 ScaledVec;
        glm_vec3_scale(Orientation, speed, ScaledVec);
        glm_vec3_add(Position, ScaledVec, Position);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        vec3 Cross;
        glm_vec3_cross(Orientation, Up, Cross);
        glm_vec3_normalize(Cross);
        glm_vec3_negate(Cross);
        glm_vec3_scale(Cross, speed, Cross);
        glm_vec3_add(Position, Cross, Position);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        vec3 Temp;
        glm_vec3_negate_to(Orientation, Temp);
        glm_vec3_scale(Temp, speed, Temp);
        glm_vec3_add(Position, Temp, Position);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        vec3 Cross;
        glm_vec3_cross(Orientation, Up, Cross);
        glm_vec3_normalize(Cross);
        glm_vec3_scale(Cross, speed, Cross);
        glm_vec3_add(Position, Cross, Position);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        vec3 ScaledVec;
        glm_vec3_scale(Up, speed, ScaledVec);
        glm_vec3_add(Position, ScaledVec, Position);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        vec3 ScaledVec;
        glm_vec3_scale(Up, speed, ScaledVec);
        glm_vec3_negate(ScaledVec);
        glm_vec3_add(Position, ScaledVec, Position);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed = 0.4f;
    }

    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        speed = 0.1f;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstClick)
        {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        double mouseX;
        double mouseY;

        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
        float rotY = sensitivity * (float)(mouseX - (height / 2)) / width;

        // glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
        vec3 NewOrientation;
        glm_vec3_dup(Orientation, NewOrientation);

        vec3 Cross;
        glm_vec3_cross(Orientation, Up, Cross);
        glm_vec3_normalize(Cross);
        glm_vec3_rotate(NewOrientation, glm_rad(-rotX), Cross);

        if (abs(glm_vec3_angle(NewOrientation, Up) - glm_rad(90.0f)) <= glm_rad(85.0f))
        {
            glm_vec3_dup(NewOrientation, Orientation);
        }

        glm_vec3_rotate(Orientation, glm_rad(-rotY), Up);

        glfwSetCursorPos(window, (width / 2), (height / 2));
    }

    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        firstClick = true;
    }
}