#include "core/Window.h"


static void resizeCallback(GLFWwindow* nWindow, int newWidth, int newHeight)
{
    Window* window = (Window*)glfwGetWindowUserPointer(nWindow);
    window->windowWidth = newWidth;
    window->windowHeight = newHeight;
    glViewport(0, 0, newWidth, newHeight);
}

void Window::setCallbackMethods() 
{
    if (appWindow != nullptr)
    {
        // Set callback method to be called everytime key event is created
        glfwSetKeyCallback(appWindow, key_callback);
        // Set callback method to be called everytime the mouse position changes
        glfwSetCursorPosCallback(appWindow, mouse_callback);
        // Set callback method to be called when the scroll wheel is used
        glfwSetScrollCallback(appWindow, scroll_callback);
        glfwSetWindowSizeCallback(appWindow, resizeCallback);
    }
}

void Window::freeWindow(Window* window) 
{
    if (window) {
        glfwDestroyWindow(window->appWindow);
        delete window;
    }
}

void Window::closeApplication() 
{
    if (appWindow != nullptr) 
    {
        glfwSetWindowShouldClose(appWindow, GLFW_TRUE);
    }
}

Window* Window::windowInit(Config& config) 
{
    // Tell GLFW the version that is being used (3.3.5)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Tell GLFW that we are using the CORE profile
    // This means we are only using the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window* res = new Window();

    GLFWmonitor* primaryMonitor = config.isFullscreen ? glfwGetPrimaryMonitor() : nullptr;

    res->appWindow = glfwCreateWindow(config.winWidth, config.winHeight, config.winTitle, primaryMonitor, nullptr);
    if (res->appWindow == nullptr) 
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(res->appWindow);
    glfwSetWindowUserPointer(res->appWindow, (void*)res);

    res->windowWidth = config.winWidth;
    res->windowHeight = config.winHeight;

    return res;
}
