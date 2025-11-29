//
//  GameOver.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/11/25.
//

#include "GameOverScene.h"
#include "../Globals.h"
#include "../Colors.h"
#include <sstream>

GameOverScene::GameOverScene() {}

void GameOverScene::initialise() {
    
}

void GameOverScene::render() {
    SDL_Renderer* renderer = Globals::getRenderer();
    
    // Draw semi-transparent overlay
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, Colors::black.r, Colors::black.g, Colors::black.b, 180);
    SDL_FRect overlay{0, 0, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)};
    SDL_RenderFillRect(renderer, &overlay);
    
    // Draw "GAME OVER" text
    LTexture gameOverText;
    if (gameOverText.loadFromRenderedText("GAME OVER", Colors::red)) {
        gameOverText.render((SCREEN_WIDTH - gameOverText.getWidth()) / 2, 150);
    }
    
    // Draw final score
    LTexture scoreText;
    std::ostringstream scoreStr;
    scoreStr << "Final Score: " << Globals::getScore();
    if (scoreText.loadFromRenderedText(scoreStr.str(), Colors::white)) {
        scoreText.render((SCREEN_WIDTH - scoreText.getWidth()) / 2, 220);
    }
    
    // Draw instruction to restart
    LTexture restartText;
    if (restartText.loadFromRenderedText("Press ESC to return to menu", Colors::lightGray)) {
        restartText.render((SCREEN_WIDTH - restartText.getWidth()) / 2, 280);
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void GameOverScene::handleEvent(SDL_Event& e) {
    Scene::handleEvent(e);
    if (e.type == SDL_EVENT_KEY_DOWN) {
        if (e.key.key == SDLK_ESCAPE) {
            Globals::setScene("initial");
        }
    }
}
