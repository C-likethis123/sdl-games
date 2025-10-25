//
//  globals.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 25/10/25.
//

//
//  globals.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 25/10/25.
//

#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <filesystem>
#include <string>

class Globals {
public:
    // Get the singleton instance
    static Globals& getInstance();

    // Get the base path (initializes on first call)
    const char* getBasePath();

    // Helper to get full resource path
    std::filesystem::path getResourcePath(const std::string& filename);
    
    // Get window
    static SDL_Window* getWindow();
    static SDL_Renderer* getRenderer();

    Globals(const Globals&) = delete;
    Globals& operator=(const Globals&) = delete;
    Globals(Globals&&) = delete;
    Globals& operator=(Globals&&) = delete;
    ~Globals() = default;

private:
    Globals() : basePath(nullptr) {
        if (!SDL_CreateWindowAndRenderer("Mai Tutorial", 640, 480, 0, &gWindow, &gRenderer)) {
            SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
    }
    const char* basePath;
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
};

// Convenience function for easy access
inline std::filesystem::path getResourcePath(const std::string& filename) {
    return Globals::getInstance().getResourcePath(filename);
}


#endif // GLOBALS_H
