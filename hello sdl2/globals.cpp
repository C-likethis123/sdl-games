//
//  globals.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 25/10/25.
//


#include "globals.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <filesystem>
#include <string>


Globals& Globals::getInstance() {
    static Globals instance;
    return instance;
}

// Get the base path (initializes on first call)
const char* Globals::getBasePath() {
    if (!basePath) {
        basePath = SDL_GetBasePath();
    }
    return basePath;
}

SDL_Window* Globals::getWindow(){
    return getInstance().gWindow;
}

SDL_Renderer* Globals::getRenderer() {
    return getInstance().gRenderer;
}

// Helper to get full resource path
std::filesystem::path Globals::getResourcePath(const std::string& filename) {
    const char* base = getBasePath();
    if (!base) {
        return filename;
    }
    return std::filesystem::path(base) / filename;
}
