#ifndef CONFIG_H
#define CONFIG_H

struct Config {
    int winWidth = 1280;
    int winHeight = 720;
    bool isFullscreen = false;
    int renderDistance = 16;
    int fov = 90;
    const char* winTitle = "VoxelCraft";
};

#endif