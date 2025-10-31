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
#include <SDL3_ttf/SDL_ttf.h>
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

struct SDL_FontDeleter {
    void operator()(TTF_Font* ptr) const {
        if (ptr) TTF_CloseFont(ptr);
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
    static std::filesystem::path getResourcePath(const std::string& filename);
    
    // Get window
    static SDL_Window* getWindow();
    static SDL_Renderer* getRenderer();
    static TTF_Font* getFont();
    
    // Get render output size (actual pixel dimensions)
    static bool getRenderOutputSize(int* width, int* height);
    
    static void setScene(const std::string& sceneKey);
    static const std::string& getSceneKey();
    
    static void setPlayer(const std::string& player);
    static const std::string& getPlayer();

    static void setEnding(const std::string& ending);
    static const std::string& getEnding();
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
    std::string sceneKey;
    std::string ending;
    std::string player;
    std::unique_ptr<SDL_Window, SDL_WindowDeleter> gWindow;
    std::unique_ptr<SDL_Renderer, SDL_RendererDeleter> gRenderer;
    std::unique_ptr<TTF_Font, SDL_FontDeleter> gFont;
};

// Convenience function for easy access
inline std::filesystem::path getResourcePath(const std::string& filename) {
    return Globals::getInstance().getResourcePath(filename);
}


#endif // GLOBALS_H
