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

bool Globals::initialise() {
    // Create window with specific flags
    SDL_Window* tempWindow = SDL_CreateWindow(
        "Mai Tutorial", 640, 480, 0);
    
    if (!tempWindow) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    // Create renderer with hardware acceleration and vsync
    SDL_Renderer* tempRenderer = SDL_CreateRenderer(
        tempWindow,
        nullptr
    );
    
    if (!tempRenderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(tempWindow);
        return false;
    }
    
    getInstance().gWindow.reset(tempWindow);
    getInstance().gRenderer.reset(tempRenderer);

    return true;
}

// Get the base path (initializes on first call)
const char* Globals::getBasePath() {
    if (!basePath) {
        basePath = SDL_GetBasePath();
    }
    return basePath;
}

SDL_Window* Globals::getWindow(){
    return getInstance().gWindow.get();
}

SDL_Renderer* Globals::getRenderer() {
    return getInstance().gRenderer.get();
}

bool Globals::getRenderOutputSize(int* width, int* height) {
    return SDL_GetCurrentRenderOutputSize(getRenderer(), width, height);
}

void Globals::cleanup() {
    // Reset unique_ptrs to trigger cleanup in proper order
    // (Renderer first, then Window, before SDL_Quit is called)
    getInstance().gRenderer.reset();
    getInstance().gWindow.reset();
}

// Helper to get full resource path
std::filesystem::path Globals::getResourcePath(const std::string& filename) {
    const char* base = getBasePath();
    if (!base) {
        return filename;
    }
    return std::filesystem::path(base) / filename;
}
