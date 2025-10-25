#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <string>
#include <filesystem>
#include "globals.h"
#include "Texture.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Current displayed image
SDL_Texture* gCurrentTexture = NULL;

LTexture gFooTexture;
LTexture gBackgroundTexture;

// Initialize SDL, window, and renderer
bool initialise() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    SDL_Renderer* gRenderer = Globals::getRenderer();

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    return true;
}



bool load_media() {
    //Loading success flag
    bool success = true;

    //Load Foo' texture
    if( !gFooTexture.loadFromFile( "foo.png" ) )
    {
        printf( "Failed to load Foo' texture image!\n" );
        success = false;
    }
    
    //Load background texture
        if( !gBackgroundTexture.loadFromFile( "background.png" ) )
        {
            printf( "Failed to load background texture image!\n" );
            success = false;
        }

    return success;
}

// Free resources and quit SDL
void close() {
//    gFooTexture.free();
//    gBackgroundTexture.free();

    if (Globals::getRenderer()) {
        SDL_DestroyRenderer(Globals::getRenderer());
    }

    if (Globals::getWindow()) {
        SDL_DestroyWindow(Globals::getWindow());
    }

    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (!initialise()) return 1;
    if (!load_media()) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to load media");
        close();
    }

    // Clear screen to white
    SDL_SetRenderDrawColor(Globals::getInstance().getRenderer(), 255, 255, 255, 255);
    SDL_RenderClear(Globals::getInstance().getRenderer());

    // Draw the texture
//    gCurrentTexture = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
    
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

                //Render background texture to screen
                gBackgroundTexture.render( 0, 0 );

                //Render Foo' to the screen
                gFooTexture.render( 240, 190 );

                //Update screen
                SDL_RenderPresent( Globals::getRenderer() );
            }
        }

    }
   

    close();
    return 0;
}
