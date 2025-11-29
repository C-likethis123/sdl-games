//
//  GameOver.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/11/25.
//


#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "Scene.h"
#include "../Tetra.h"
#include <vector>
#include <memory>
#include "../Texture.h"
#include <SDL3/SDL_main.h>

class GameOverScene : public Scene {
public:
    GameOverScene();
    
    void initialise() override;
    void render() override;
    void handleEvent(SDL_Event& e) override;
    void reset();  // Reset game state
    
private:
    // Helper methods
    void renderGameOver();

    LTexture background;
};

#endif // GAMESCENE_H
