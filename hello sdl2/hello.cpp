#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <vector>
#include "globals.h"
#include "Texture.h"




// Refactoring goal: get rid of global constants
LTexture gSpriteSheetTexture;
std::vector<std::pair<SDL_FRect, SDL_FRect>> gSpriteClips;

// Refactoring goal: getRenderer() is not obvious that the window is also initialised
// Initialize SDL, window, and renderer
bool initialise() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    if (!Globals::initialise()) {
        SDL_Log("Globals could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(Globals::getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);

    return true;
}



bool load_media() {
    //Loading success flag
    bool success = true;

    //Load Foo' texture
    if( !gSpriteSheetTexture.loadFromFile( "sprites.png" ) )
    {
        printf( "Failed to load sprites.png\n" );
        success = false;
    }
    
    // Get actual screen dimensions
    int screenWidth = 640;
    int screenHeight = 480;
    
    // Initialize sprite positions based on actual screen size
    gSpriteClips = {
        {SDL_FRect{0, 0, 0, 0}, SDL_FRect{0, 0, 100, 100}},  // Top-left
        {SDL_FRect{float(screenWidth-100), 0, 0, 0}, SDL_FRect{100, 0, 100, 100}},  // Top-right
        {SDL_FRect{0, float(screenHeight-100), 0, 0}, SDL_FRect{0, 100, 100, 100}},  // Bottom-left
        {SDL_FRect{float(screenWidth-100), float(screenHeight-100), 0, 0}, SDL_FRect{100, 100, 100, 100}},  // Bottom-right
    };

    return success;
}

// Free resources and quit SDL
void close() {
    // Cleanup resources in proper order (Renderer, Window, then SDL)
    Globals::cleanup();
    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (!initialise()) return 1;
    if (!load_media()) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to load media");
        close();
    }

    // Clear screen to white
    SDL_SetRenderDrawColor(Globals::getRenderer(), 255, 255, 255, 255);
    SDL_RenderClear(Globals::getRenderer());

    
    bool quit = false;
    SDL_Event e;
    
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            } else {
                //Clear screen
                SDL_SetRenderDrawColor( Globals::getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( Globals::getRenderer() );

                for (const auto& rect : gSpriteClips) {
                    gSpriteSheetTexture.render(rect.first.x, rect.first.y, &rect.second);
                }

                //Update screen
                SDL_RenderPresent( Globals::getRenderer() );
            }
        }

    }
   

    close();
    return 0;
}
