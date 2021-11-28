#include <iostream>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

#include "mesh.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int width = 800;
const unsigned int height = 800;

const std::string shadersPath = "\\resources\\shaders\\";
const std::string texturesPath = "\\resources\\textures\\";

// get path to this project
std::string parentPath = std::filesystem::current_path().parent_path().string();

// Vertices coordinates
Vertex vertices[] =
    { //       /    COORDINATES    /        COLORS      /    TexCoord      /    NORMALS   //
        Vertex{{-1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        Vertex{{-1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        Vertex{{1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        Vertex{{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}
    };

// Indices for vertices order
GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

Vertex lightVertices[] =
    {
        Vertex{{-0.1f, -0.1f, 0.1f}},
        Vertex{{-0.1f, -0.1f, -0.1f}},
        Vertex{{0.1f, -0.1f, -0.1f}},
        Vertex{{0.1f, -0.1f, 0.1f}},
        Vertex{{-0.1f, 0.1f, 0.1f}},
        Vertex{{-0.1f, 0.1f, -0.1f}},
        Vertex{{0.1f, 0.1f, -0.1f}},
        Vertex{{0.1f, 0.1f, 0.1f}}
    };

GLuint lightIndices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
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
    
    std::cout << parentPath + texturesPath + "minecraft-wood-plank-texture.png" << std::endl;

    // Textures
    Texture textures[] = {
        Texture((parentPath + texturesPath + "planks.png").c_str(), "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture((parentPath + texturesPath + "planksSpec.png").c_str(), "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };

    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram((parentPath + shadersPath + "default.vert").c_str(), (parentPath + shadersPath + "default.frag").c_str());
    // Store mesh data in vectors for the mesh
    std::vector<Vertex> verts(vertices, vertices + sizeof(vertices)/sizeof(Vertex));
    std::vector<GLuint> ind(indices, indices + sizeof(indices)/sizeof(GLuint));
    std::vector<Texture> tex(textures, textures + sizeof(textures)/sizeof(Texture));
    // Create floor mesh
    Mesh floor(verts, ind, tex);

    // Generates Shader object using shaders light.vert and light.frag
    Shader lightShader((parentPath + shadersPath + "light.vert").c_str(), (parentPath + shadersPath + "light.frag").c_str());
    // Store mesh data in vectors for mesh
    std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices)/sizeof(Vertex));
    std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices)/sizeof(GLuint));
    // create floor mesh
    Mesh light(lightVerts, lightInd, tex);

    vec4 lightColor = GLM_VEC4_ONE_INIT;
    vec3 lightPos = {0.5f, 0.5f, 0.5f};
    mat4 lightModel = GLM_MAT4_IDENTITY_INIT;
    glm_translate(lightModel, lightPos);

    vec3 objectPos = GLM_VEC3_ZERO_INIT;
    mat4 objectModel = GLM_MAT4_IDENTITY_INIT;
    glm_translate(objectModel, objectPos);

    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, lightModel[0]);
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor[0], lightColor[1], lightColor[2], lightColor[3]);
    shaderProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, objectModel[0]);
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor[0], lightColor[1], lightColor[2], lightColor[3]);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos[0], lightPos[1], lightPos[2]);


    // Enables the depth buffer
    glEnable(GL_DEPTH_TEST);
  
    // position vector for camera object
    vec3 position = {0.0f, 0.0f, 2.0f};

    // Creates camera object
    Camera camera(width, height,position);

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Handles Camera Inputs
        camera.Inputs(window);
        // Updates and exports the camera matrix to the Vertex Shader
        camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

       floor.Draw(shaderProgram, camera);
       light.Draw(lightShader, camera);

        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // take care of all GLFW events
        glfwPollEvents();
    }

    shaderProgram.Delete();
    lightShader.Delete();

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
}

// update the viewport after window changes size
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}