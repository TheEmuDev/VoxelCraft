#include <iostream>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

#include "texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int width = 800;
const unsigned int height = 800;

const std::string shadersPath = "\\resources\\shaders\\";
const std::string texturesPath = "\\resources\\textures\\";

float dltRot = 0.5f;

vec3 yUp = {0.0f, 1.0f, 0.0f};
vec3 temp = {0.0f, -0.5f, -2.0f};

// Vertice Coordinates
GLfloat vertices[] =
    {
        -0.5f, 0.0f,  0.5f,      0.83f, 0.70f, 0.44f,     0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f,      0.83f, 0.70f, 0.44f,     5.0f, 0.0f,
         0.5f, 0.0f, -0.5f,      0.83f, 0.70f, 0.44f,     0.0f, 0.0f,
         0.5f, 0.0f,  0.5f,      0.83f, 0.70f, 0.44f,     5.0f, 0.0f,
         0.0f, 0.8f,  0.0f,      0.92f, 0.86f, 0.76f,     2.5f, 5.0f
    };

// Indices for vertices order
GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
};

int main()
{
    // Initialize GLFW
    // Exit with error message and -1 return code if initialization fails
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Tell GLFW the version that is being used (3.3.5)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Tell GLFW that we are using the CORE profile
    // This means we are only using the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window object with length and width of 500 pixels and the name "VoxelCraft"
    GLFWwindow *window = glfwCreateWindow(width, height, "VoxelCraft", NULL, NULL);

    // If window object fails to initialize,
    // Exit with error message and -1 return code if initialization fails
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Introduce window into current context
    glfwMakeContextCurrent(window);

    // Set callback method to be called everytime window is resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Load GLAD so it configures OpenGL
    gladLoadGL();

    // Specify the viewport of OpenGL in the Window
    // In this case the viewport goes from (0,0) to (800,800)
    glViewport(0, 0, width, height);

    // Generates Shader object using shaders default.vert and default.frag
    std::string parentPath = std::filesystem::current_path().parent_path().string();
    Shader shaderProgram((parentPath + shadersPath + "default.vert").c_str(), (parentPath + shadersPath + "default.frag").c_str());

    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, sizeof(vertices));

    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices, sizeof(indices));

    // Links VBO to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    std::string texPath = parentPath + texturesPath + "grass_block_test.png";
    Texture testImage(texPath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    testImage.TexUnit(shaderProgram, "tex0", 0);

    float rotation = 0.0f;
    
    double prevTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Tell OpenGL which Shader Program to use
        shaderProgram.Activate();

        double currentTime = glfwGetTime();
        if(currentTime - prevTime >= 1/60)
        {
            rotation += dltRot;
            prevTime = currentTime;
        }

        mat4 model = GLM_MAT4_IDENTITY_INIT;
        mat4 view = GLM_MAT4_IDENTITY_INIT;
        mat4 proj = GLM_MAT4_IDENTITY_INIT;

        glm_rotate(model, glm_rad(rotation), yUp);
        glm_translate(view, temp);
        glm_perspective(glm_rad(45.0f), (float)width/height, 0.1f, 100.0f, proj);

        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model[0]);
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view[0]);
        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj[0]);

        // Assigns a value to the uniform; NOTE: Must always be done after activating the shader program
        glUniform1f(uniID, 0.5f);
        // Binds texture so that it appears in rendering
        testImage.Bind();
        // Binds the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // take care of all GLFW events
        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    testImage.Delete();
    shaderProgram.Delete();

    // Delete window before ending the program
    glfwDestroyWindow(window);

    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}

// Handle input events
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        dltRot = -0.5f;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        dltRot = 0.5f;
    }
}

// update the viewport after window changes size
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}