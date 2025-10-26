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
LTexture gWalkingManTexture;
// will need to figure out how to associate a sprite with a constant
std::vector<std::pair<SDL_FRect, SDL_FRect>> gSpriteClips;
std::vector<SDL_FRect> gWalkingSpriteClips;

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
    // Get actual screen dimensions
    int screenWidth = 640;
    int screenHeight = 480;
    //Loading success flag
    bool success = true;

    //Load Foo' texture
    if( !gSpriteSheetTexture.loadFromFile( "sprites.png" ) )
    {
        printf( "Failed to load sprites.png\n" );
        success = false;
    }
    // Initialize sprite positions based on actual screen size
    gSpriteClips = {
        {SDL_FRect{0, 0, 0, 0}, SDL_FRect{0, 0, 100, 100}},  // Top-left
        {SDL_FRect{float(screenWidth-100), 0, 0, 0}, SDL_FRect{100, 0, 100, 100}},  // Top-right
        {SDL_FRect{0, float(screenHeight-100), 0, 0}, SDL_FRect{0, 100, 100, 100}},  // Bottom-left
        {SDL_FRect{float(screenWidth-100), float(screenHeight-100), 0, 0}, SDL_FRect{100, 100, 100, 100}},  // Bottom-right
    };
    
    if (!gWalkingManTexture.loadFromFile( "walking.png") ) {
        success = false;
    }
    
    // Initialize sprite positions based on actual screen size
    gWalkingSpriteClips = {
        SDL_FRect{0, 0, 64, 205},  // Top-left
        SDL_FRect{64, 0, 64, 205},  // Top-right
        SDL_FRect{128, 0, 64, 205},  // Bottom-left
        SDL_FRect{192, 0, 64, 205}  // Bottom-right
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
    int CURRENT_WALKING_MAN = 0;
    uint64_t start_time = SDL_GetTicks();
    
    //Angle of rotation
    double degrees = 0;

    //Flip type
    SDL_FlipMode flipType = SDL_FLIP_NONE;
    
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            } else if (e.type == SDL_EVENT_KEY_DOWN) {
                switch (e.key.key) {
                    case (SDLK_A):
                    degrees -= 60;
                    break;
                    case SDLK_D:
                    degrees += 60;
                    break;

                    case SDLK_Q:
                    flipType = SDL_FLIP_HORIZONTAL;
                    break;

                    case SDLK_W:
                    flipType = SDL_FLIP_NONE;
                    break;

                    case SDLK_E:
                    flipType = SDL_FLIP_VERTICAL;
                    break;
                    default:
                        flipType = SDL_FLIP_NONE;
                }
            }
        }
        //Clear screen
        SDL_SetRenderDrawColor( Globals::getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( Globals::getRenderer() );

        for (const auto& rect : gSpriteClips) {
            gSpriteSheetTexture.render(rect.first.x, rect.first.y, &rect.second);
        }
        SDL_FRect& rect = gWalkingSpriteClips[CURRENT_WALKING_MAN];
        gWalkingManTexture.render((640 - rect.w) / 2, (480 - rect.h) / 2, &rect, degrees, NULL, flipType);
        
        uint64_t end_time = SDL_GetTicks();
        if (end_time - start_time >= 120) {
            CURRENT_WALKING_MAN = (CURRENT_WALKING_MAN + 1) % 4;
            start_time = end_time;
        }

        //Update screen
        SDL_RenderPresent( Globals::getRenderer() );
        

    }
   

    close();
    return 0;
}
