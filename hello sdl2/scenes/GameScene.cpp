//
//  GameScene.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 30/10/25.
//

#include "GameScene.h"

GameScene::GameScene() {
    initialise();  // Call initialise after construction
}

void GameScene::initialise() {
    SDL_Color textColor = { 0, 0, 0 };
    if( !background.loadFromRenderedText( "The game page", textColor ) ) {
        printf( "Failed to render text texture!\n" );
    }
}

void GameScene::render() {
    background.render( ( SCREEN_WIDTH - background.getWidth() ) / 2, ( SCREEN_HEIGHT - background.getHeight() ) / 2 );
}

