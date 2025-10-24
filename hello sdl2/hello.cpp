#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;

// Initialize SDL, window, and renderer
bool initialise() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("Mai Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!gWindow) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, nullptr);
    if (!gRenderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

// Helper to get full resource path
std::string getResourcePath(const std::string& filename) {
    const char* base = SDL_GetBasePath();
    if (!base) return filename;
    std::string path = std::string(base) + filename;
    SDL_free((void*)base);
    return path;
}

// Load BMP and create a texture
bool load_media() {
    std::string path = getResourcePath("hello_world.bmp");
    SDL_Log("Loading image at: %s\n", path.c_str());

    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    if (!loadedSurface) {
        SDL_Log("Unable to load image! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Create texture from surface
    gTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_DestroySurface(loadedSurface);

    if (!gTexture) {
        SDL_Log("Unable to create texture! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

// Free resources and quit SDL
void close() {
    if (gTexture) {
        SDL_DestroyTexture(gTexture);
        gTexture = nullptr;
    }

    if (gRenderer) {
        SDL_DestroyRenderer(gRenderer);
        gRenderer = nullptr;
    }

    if (gWindow) {
        SDL_DestroyWindow(gWindow);
        gWindow = nullptr;
    }

    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (!initialise()) return 1;

    if (!load_media()) {
        close();
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                quit = true;
        }

        // Clear screen to white
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);

        // Draw the texture
        SDL_RenderTexture(gRenderer, gTexture, nullptr, nullptr);

        // Present to screen
        SDL_RenderPresent(gRenderer);
    }

    close();
    return 0;
}
