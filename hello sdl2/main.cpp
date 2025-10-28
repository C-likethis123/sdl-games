#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <vector>
#include "Globals.h"
#include "Texture.h"


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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
    // Get actual screen dimensions
    int screenWidth = 640;
    int screenHeight = 480;
    //Loading success flag
    bool success = true;

    //Load Foo' texture
//    if( !gSpriteSheetTexture.loadFromFile( "sprites.png" ) )
//    {
//        printf( "Failed to load sprites.png\n" );
//        success = false;
//    }
    
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
    uint64_t start_time = SDL_GetTicks();
    SDL_FRect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }
        
        // TODO: handle events here

        //Clear screen
        SDL_SetRenderDrawColor( Globals::getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( Globals::getRenderer() );
        
        // Update actions and render again
        
        //Update screen
        SDL_RenderPresent( Globals::getRenderer() );
        

    }
   

    close();
    return 0;
}
