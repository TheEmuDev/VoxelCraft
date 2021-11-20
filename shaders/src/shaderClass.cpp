#include "shaderClass.h"

std::string get_file_contents(const char *filename)
{
    // Reads a text file and outputs a string with everything in the text file
    std::ifstream in(filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return (contents);
    }
    throw(errno);
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char *vertexFile, const char *fragmentFile)
{
    // Read a vertexFile and fragmentFile and store the strings
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    // Convert the shader source strings into character arrays
    const char *vertexSource = vertexCode.c_str();
    const char *fragmentSource = fragmentCode.c_str();

    // Create Vertex Shader Object and get reference
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach Vertex Shader source to the Vertex Shader Object
    glShaderSource(vertexShader, 1, &vertexSource, NULL);

    // Compile the vertex shader into machine code
    glCompileShader(vertexShader);

    // Check if Shader comiled successfully
    compileErrors(vertexShader, "VERTEX");

    // Create Fragment Shader Object and get its reference
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Attach Fragment Shader source to the Fragment Shader object
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

    // Compile the Fragment Shader into machine code
    glCompileShader(fragmentShader);

    // Check if Shader compiled successfully
    compileErrors(fragmentShader, "FRAGMENT");

    // Create Shader Program Object and get its reference
    ID = glCreateProgram();

    // Attach the Vertex and Fragment Shaders together into the Shader Program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    // Link all the shaders together into the shader Program
    glLinkProgram(ID);

    // Check if Shaders linked correctly
    compileErrors(ID, "PROGRAM");

    // Delete now useless Vertex and Fragment Shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
    glUseProgram(ID);
}

void Shader::Delete()
{
    glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type) 
{
    const int logLength = 1024;

    // stores status of compilation
    GLint hasCompiled;
    // Character Array to store error message in
    char infoLog[logLength];

    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if(hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, logLength, NULL, infoLog);
            std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << std::endl;
        }
    }

    else
        {
            glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
            if(hasCompiled == GL_FALSE)
            {
                glGetProgramInfoLog(shader, logLength, NULL, infoLog);
                std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << std::endl;
            }
        }
}