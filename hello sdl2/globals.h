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
#include <memory>

// Custom deleters for SDL resources
struct SDL_CharDeleter {
    void operator()(char* ptr) const {
        if (ptr) SDL_free(ptr);
    }
};

struct SDL_WindowDeleter {
    void operator()(SDL_Window* ptr) const {
        if (ptr) SDL_DestroyWindow(ptr);
    }
};

struct SDL_RendererDeleter {
    void operator()(SDL_Renderer* ptr) const {
        if (ptr) SDL_DestroyRenderer(ptr);
    }
};

class Globals {
public:

    // initialise the globals
    static bool initialise();
    // Get the singleton instance
    static Globals& getInstance();

    // Get the base path (initializes on first call)
    const char* getBasePath();

    // Helper to get full resource path
    std::filesystem::path getResourcePath(const std::string& filename);
    
    // Get window
    static SDL_Window* getWindow();
    static SDL_Renderer* getRenderer();
    
    // Get render output size (actual pixel dimensions)
    static bool getRenderOutputSize(int* width, int* height);
    
    // Cleanup resources (call before SDL_Quit)
    static void cleanup();

    Globals(const Globals&) = delete;
    Globals& operator=(const Globals&) = delete;
    Globals(Globals&&) = delete;
    Globals& operator=(Globals&&) = delete;
    ~Globals() = default;

private:
    Globals() : basePath(nullptr), gWindow(nullptr), gRenderer(nullptr) {}
    
    const char* basePath;
    std::unique_ptr<SDL_Window, SDL_WindowDeleter> gWindow;
    std::unique_ptr<SDL_Renderer, SDL_RendererDeleter> gRenderer;
};

// Convenience function for easy access
inline std::filesystem::path getResourcePath(const std::string& filename) {
    return Globals::getInstance().getResourcePath(filename);
}


#endif // GLOBALS_H
