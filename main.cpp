#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

// Vertice Coordinates
GLfloat vertices[] = {
    -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,    // Lower left corner
    0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,     // Lower right corner
    0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,  // Upper corner
    -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
    0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,  // Inner right
    0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f      // Inner down
};

GLuint indices[] = {
    0, 3, 5, // lower left triangle
    3, 2, 4, // lower right triangle
    5, 4, 1  // upper triangle
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

    // Create window object with length and width of 800 pixels and the name "VoxelCraft"
    GLFWwindow *window = glfwCreateWindow(800, 800, "VoxelCraft", NULL, NULL);

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

    //Load GLAD so it configures OpenGL
    gladLoadGL();

    // Specify the viewport of OpenGL in the Window
    // In this case the viewport goes from (0,0) to (800,800)
    glViewport(0, 0, 800, 800);


    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram("shaders/resources/default.vert", "shaders/resources/default.frag");

    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, sizeof(vertices));

    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices, sizeof(indices));

    // Links VBO to VAO
    VAO1.LinkVBO(VBO1, 0);

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
    
    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.Activate();

        VAO1.Bind();

        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();

    // Delete window before ending the program
    glfwDestroyWindow(window);

    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}