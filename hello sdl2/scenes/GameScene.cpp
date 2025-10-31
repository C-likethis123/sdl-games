//
//  GameScene.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 30/10/25.
//

#include "GameScene.h"
#include "../Globals.h"
#include <sstream>

GameScene::GameScene() {
    initialise();  // Call initialise after construction
}

void GameScene::initialise() {
    // Grid positions based on the rendered lines:
    // Vertical lines at x=220 and x=420
    // Horizontal lines at y=180 and y=300
    // Grid spans from (100, 100) to (540, 400)
    
    // Calculate center positions for each cell
    // Cell dimensions: 120x80 approximately
    struct GridPos {
        float x, y;
    };
    
    GridPos positions[9] = {
        // Row 1 (y: 100-180)
        {100.0f, 100.0f}, {220.0f, 100.0f}, {420.0f, 100.0f},
        // Row 2 (y: 180-300)
        {100.0f, 180.0f}, {220.0f, 180.0f}, {420.0f, 180.0f},
        // Row 3 (y: 300-400)
        {100.0f, 300.0f}, {220.0f, 300.0f}, {420.0f, 300.0f}
    };
    
    // Create 9 buttons for the grid
    for (int i = 0; i < 9; i++) {
        // Initial text is empty, will be filled when clicked
        // Cell widths: first and last columns are 120px, middle column is 200px
        float cellWidth = (i % 3 == 1) ? 200.0f : 120.0f;
        float cellHeight = (i < 3) ? 80.0f : ((i < 6) ? 120.0f : 100.0f);
        
        SDL_FRect buttonRect = {positions[i].x, positions[i].y, cellWidth, cellHeight};
        
        // Capture this and i to access buttons vector
        auto buttonCallback = [this, i]() {
            // Only allow clicking empty cells
            if (buttons[i].getText().empty()) {
                // Set the button text to current player
                buttons[i].setText(Globals::getPlayer() == "cross" ? "X" : "O");
                
                // TODO: Check for win condition
                
                // Switch player
                if (Globals::getPlayer() == "cross") {
                    Globals::setPlayer("circle");
                } else {
                    Globals::setPlayer("cross");
                }
            }
        };
        
        // Use emplace_back to construct Button in place
        buttons.emplace_back("", buttonRect, buttonCallback);
    }
}

// padding of 20 px
// each is 200 in length

// should a grid be a button?
void GameScene::render() {
    
    // display the current player on top!
    SDL_Color textColor = { 0, 0, 0 };
    std::ostringstream backgroundText;
    backgroundText << "Current player: " << Globals::getPlayer();
    // render 3 lines and treat them as buttons
    // when clicked, show the button text
    if( !background.loadFromRenderedText( backgroundText.str(), textColor ) ) {
        printf( "Failed to render text texture!\n" );
    }
    background.render( 100, 50 );
    
    // renders the grid
    SDL_SetRenderDrawColor(Globals::getRenderer(), 0, 0, 0, 0);
    
    // vertical lines
    SDL_RenderLine(Globals::getRenderer(), 220, 100, 220, 400);
    SDL_RenderLine(Globals::getRenderer(), 420, 100, 420, 400);
    
    // horizontal lines
    SDL_RenderLine(Globals::getRenderer(), 100, 180, 540, 180);
    SDL_RenderLine(Globals::getRenderer(), 100, 300, 540, 300);
    
    // Render all grid buttons
    for (auto& button : buttons) {
        button.render();
    }
}

