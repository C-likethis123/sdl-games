//
//  globals.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 25/10/25.
//


#include "Globals.h"
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
    // somehow SDL does not have a SDL_RENDERER_VSYNC flag that enables vsync.
    SDL_SetRenderVSync(tempRenderer, 1);
    
    TTF_Font* tempFont = TTF_OpenFont( Globals::getResourcePath("lazy.ttf").c_str(), 28 );
    if (!tempFont) {
        SDL_Log("TTF cannot be loaded! SDL_Error: %s\n", SDL_GetError());
    }
    
    getInstance().gWindow.reset(tempWindow);
    getInstance().gRenderer.reset(tempRenderer);
    getInstance().gFont.reset(tempFont);

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

TTF_Font* Globals::getFont() {
    return getInstance().gFont.get();
}


bool Globals::getRenderOutputSize(int* width, int* height) {
    return SDL_GetCurrentRenderOutputSize(getRenderer(), width, height);
}

void Globals::cleanup() {
    // Reset unique_ptrs to trigger cleanup in proper order
    // (Font first, then Renderer, then Window, before SDL_Quit is called)
    getInstance().gFont.reset();
    getInstance().gRenderer.reset();
    getInstance().gWindow.reset();
    
    // Quit SDL_ttf
    TTF_Quit();
}

// Helper to get full resource path
std::filesystem::path Globals::getResourcePath(const std::string& filename) {
    const char* base = getInstance().getBasePath();
    if (!base) {
        return filename;
    }
    return std::filesystem::path(base) / filename;
}
