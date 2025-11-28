//
//  InitialScene.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 1/11/25.
//

#include "InitialScene.h"
#include "../Globals.h"
InitialScene::InitialScene() {
    initialise();
}

void InitialScene::initialise() {
    if (!background.loadFromRenderedText("Tetris", SDL_Color{0,0,0})) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Error logging background text: Tetris, %s", SDL_GetError());
    }
    buttons.emplace_back("Start", SDL_FRect{(SCREEN_WIDTH - background.getWidth()) / 2, (SCREEN_HEIGHT - 30) / 4 * 3, background.getWidth(), 30}, []() {
        Globals::setScene("next");
    });
}

void InitialScene::render() {
    background.render( ( SCREEN_WIDTH - background.getWidth() ) / 2, ( SCREEN_HEIGHT - background.getHeight() ) / 4 );
    for (auto& button: buttons) {
        button.render();
    }
}
