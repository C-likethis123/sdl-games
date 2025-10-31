//
//  EndingScene.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 31/10/25.
//

#include "EndingScene.h"
#include "../Globals.h"
#include <sstream>

EndingScene::EndingScene() : startTime(0) {
    
}

void EndingScene::initialise() {
    // empty
}

void EndingScene::onEnter() {
    // Record the time when this scene becomes active
    startTime = SDL_GetTicks();
}

void EndingScene::render() {
    // Check if 5 seconds have elapsed
    uint64_t currentTime = SDL_GetTicks();
    uint64_t elapsedTime = currentTime - startTime;
    
    if (elapsedTime >= DISPLAY_DURATION_MS) {
        // Time's up, go back to initial scene
        Globals::setScene("initial");
        return;
    }
    
    // Display the winner message
    SDL_Color textColor = { 0, 0, 0 };
    std::ostringstream backgroundText;
    if (Globals::getEnding() == "draw") {
        backgroundText << "It's a draw!";
    } else {
        backgroundText << Globals::getPlayer() << " wins!";
    }
    
    if( !background.loadFromRenderedText( backgroundText.str(), textColor ) ) {
        printf( "Failed to render text texture!\n" );
    }
    background.render( ( SCREEN_WIDTH - background.getWidth() ) / 2, ( SCREEN_HEIGHT - background.getHeight() ) / 2 );
}
