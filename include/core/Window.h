#ifndef WINDOW_H
#define WINDOW_H

#include "Core.h"
#include "Config.h"
#include <iostream>

struct Window{
    GLFWwindow* appWindow;
    int windowWidth;
    int windowHeight;

    void setCallbackMethods();
    void closeApplication();

    static Window* windowInit(Config& config);
    static void freeWindow(Window* window);
};

#endif