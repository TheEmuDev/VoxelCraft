#include <iostream>
#include <filesystem>

#include "core.h"
#include "core/Window.h"
#include "core/Input.h"
#include "../include/gfx/Model.h"

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

 
const std::string SHADER_PATH = "\\resources\\shaders\\";
const std::string TEXTURE_PATH = "\\resources\\textures\\";
const std::string WINDOW_TITLE = "VoxelCraft";

Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));


bool renderAsWireframe = false;
bool firstMouse = true;

double previousTime = 0.0;
double currentTime = 0.0;
double deltaTime;

float lastX = SCREEN_WIDTH / 2;
float lastY = SCREEN_HEIGHT / 2;


int main()
{
    // Initialize GLFW
    // Exit with error message and -1 return code if initialization fails
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    Window* window = Window::windowInit();


    // Introduce window into current context
    
    

    //Load GLAD so it configures OpenGL
    gladLoadGL();

    // Specify the viewport of OpenGL in the Window
    // In this case the viewport goes from (0,0) to (800,800)
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram("default.vert", "default.frag");

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPosition = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPosition);

    shaderProgram.Activate();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);

    // Enables the depth buffer
    glEnable(GL_DEPTH_TEST);

    unsigned int frameCounter = 0;
    

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        deltaTime = currentTime - previousTime;
        frameCounter++;

        if(deltaTime >= 1.0/3.0)
        {
            std::string FPS = std::to_string((1.0 / deltaTime) * frameCounter);
            std::string ms = std::to_string((deltaTime / frameCounter) * 1000);
            std::string stats = " - " + FPS + "FPS / " + ms + "ms";

            glfwSetWindowTitle(window, (WINDOW_TITLE + stats).c_str());

            previousTime = currentTime;
            frameCounter = 0;
        }

        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Handles Camera Inputs
        camera.ProcessKeyboardInputs(window, deltaTime);
        // Updates and exports the camera matrix to the Vertex Shader
        camera.UpdateMatrix(0.1f, 100.0f);
        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // take care of all GLFW events
        glfwPollEvents();
    }

    shaderProgram.Delete();

    // Delete window before ending the program
    glfwDestroyWindow(window);

    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}

// update the viewport after window changes size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_RELEASE) return;
    if(key == GLFW_KEY_F1)
    {
        renderAsWireframe = !renderAsWireframe;
        unsigned int renderMode;

        (renderAsWireframe) ? renderMode = GL_LINE : renderMode = GL_FILL;
        glPolygonMode(GL_FRONT_AND_BACK, renderMode);
    }

    if(key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if(firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;   // reversed because y-coordinates go from bottom to top

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.ProcessMouseScroll(yOffset);
}