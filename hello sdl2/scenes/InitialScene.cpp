//
//  InitialScene.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 30/10/25.
//

#include "InitialScene.h"
#include "../Globals.h"

InitialScene::InitialScene() {
    initialise();  // Call initialise after construction
}

void InitialScene::initialise() {
    SDL_Color textColor = { 0, 0, 0 };
    if( !background.loadFromRenderedText( "Tic Tac Toe", textColor ) ) {
        printf( "Failed to render text texture!\n" );
    }
    buttons.emplace_back("Cross", []() {
        Globals::setScene("next");
    });
};
void InitialScene::render() {
    background.render( ( SCREEN_WIDTH - background.getWidth() ) / 2, ( SCREEN_HEIGHT - background.getHeight() ) / 2 );
    for (auto& button : buttons) {
        button.render();
    }
};

