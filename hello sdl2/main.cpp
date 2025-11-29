#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <vector>
#include <memory>

#include "Globals.h"
#include "Colors.h"
#include "scenes/Scene.h"
#include "scenes/InitialScene.h"
#include "scenes/GameScene.h"
#include "scenes/GameOverScene.h"

/**
 
 Refactoring opportunities:
 1. How to position elements better other than hardcoding?
 2. How to resize widgets according to elements?
 
 */

// Initialize SDL, window, and renderer
bool initialise() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    //Initialize SDL_ttf
    if( !TTF_Init() )
    {
        SDL_Log( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", SDL_GetError() );
        return false;
    }

    if (!Globals::initialise()) {
        SDL_Log("Globals could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    

    SDL_SetRenderDrawColor(Globals::getRenderer(), Colors::white.r, Colors::white.g, Colors::white.b, Colors::white.a);

    return true;
}


// Free resources and quit SDL
void close() {
    // Cleanup resources in proper order (Renderer, Window, then SDL)
    Globals::cleanup();
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (!initialise()) return 1;

    // Clear screen to white
    SDL_SetRenderDrawColor(Globals::getRenderer(), Colors::white.r, Colors::white.g, Colors::white.b, Colors::white.a);
    SDL_RenderClear(Globals::getRenderer());

    
    bool quit = false;
    SDL_Event e;

    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
    scenes.emplace("initial", std::make_unique<InitialScene>());
    scenes.emplace("next", std::make_unique<GameScene>());
    scenes.emplace("gameover", std::make_unique<GameOverScene>());
    
    std::string previousSceneKey = Globals::getSceneKey();
    
    while (!quit) {
        std::string currentSceneKey = Globals::getSceneKey();
        
        Scene& scene = *scenes[currentSceneKey];
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            } else {
                scene.handleEvent(e);
            }
        }
         //Clear screen
         SDL_SetRenderDrawColor( Globals::getRenderer(), Colors::white.r, Colors::white.g, Colors::white.b, Colors::white.a );
         SDL_RenderClear( Globals::getRenderer() );
         
         // Update actions and render again
         scene.render();
         
         //Update screen
         SDL_RenderPresent( Globals::getRenderer() );
    }
   

    close();
    return 0;
}
