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
    // Create 9 buttons for the grid
    for (int i = 0; i < 9; i++) {
        int row = i / 3;  // 0, 1, 2
        int col = i % 3;  // 0, 1, 2
        
        float x = GRID_START_X + (col * CELL_SIZE);
        float y = GRID_START_Y + (row * CELL_SIZE);
        
        SDL_FRect buttonRect = {x, y, CELL_SIZE, CELL_SIZE};
        
        // Capture this and i to access buttons vector
        auto buttonCallback = [this, i]() {
            // Only allow clicking empty cells
            if (buttons[i].getText().empty()) {
                // Get current player symbol
                std::string playerSymbol = (Globals::getPlayer() == "cross") ? "X" : "O";
                
                // Set the button text to current player
                buttons[i].setText(playerSymbol);
                
                // Check for win condition
                if (checkWin(playerSymbol)) {
                    Globals::setEnding(Globals::getPlayer());
                    Globals::setScene("ending");
                    for (int i = 0; i < 9; i++) {
                        buttons[i].setText("");
                    }
                    return;
                }
                
                // Check for draw (all cells filled)
                bool isFull = true;
                for (const auto& button : buttons) {
                    if (button.getText().empty()) {
                        isFull = false;
                        break;
                    }
                }
                if (isFull) {
                    Globals::setEnding("draw");
                    Globals::setScene("ending");
                    for (int i = 0; i < 9; i++) {
                        buttons[i].setText("");
                    }
                    return;
                }
                
                // Switch player
                if (Globals::getPlayer() == "cross") {
                    Globals::setPlayer("circle");
                } else {
                    Globals::setPlayer("cross");
                }
            }
        };
        
        buttons.emplace_back("", buttonRect, buttonCallback);
    }
}

// padding of 20 px
// each is 200 in length

// should a grid be a button?
void GameScene::render() {
    
    // Display the current player text, centered horizontally
    SDL_Color textColor = { 0, 0, 0 };
    std::ostringstream backgroundText;
    backgroundText << "Current player: " << Globals::getPlayer();
    
    if( !background.loadFromRenderedText( backgroundText.str(), textColor ) ) {
        printf( "Failed to render text texture!\n" );
    }
    // Center the text horizontally
    int textX = (SCREEN_WIDTH - background.getWidth()) / 2;
    background.render( textX, 50 );
    
    // Render the grid (300x300 with equal 100x100 cells, centered)
    SDL_SetRenderDrawColor(Globals::getRenderer(), 0, 0, 0, 0);
    
    // Vertical lines
    SDL_RenderLine(Globals::getRenderer(), 
                   GRID_START_X + CELL_SIZE, GRID_START_Y, 
                   GRID_START_X + CELL_SIZE, GRID_START_Y + GRID_SIZE);
    SDL_RenderLine(Globals::getRenderer(), 
                   GRID_START_X + 2 * CELL_SIZE, GRID_START_Y, 
                   GRID_START_X + 2 * CELL_SIZE, GRID_START_Y + GRID_SIZE);
    
    // Horizontal lines
    SDL_RenderLine(Globals::getRenderer(), 
                   GRID_START_X, GRID_START_Y + CELL_SIZE, 
                   GRID_START_X + GRID_SIZE, GRID_START_Y + CELL_SIZE);
    SDL_RenderLine(Globals::getRenderer(), 
                   GRID_START_X, GRID_START_Y + 2 * CELL_SIZE, 
                   GRID_START_X + GRID_SIZE, GRID_START_Y + 2 * CELL_SIZE);
    
    // Render all grid buttons
    for (auto& button : buttons) {
        button.render();
    }
}

bool GameScene::checkWin(const std::string& player) {
    // Check all rows
    for (int row = 0; row < 3; row++) {
        int startIndex = row * 3;  // Row 0: index 0, Row 1: index 3, Row 2: index 6
        if (buttons[startIndex].getText() == player &&
            buttons[startIndex + 1].getText() == player &&
            buttons[startIndex + 2].getText() == player) {
            return true;
        }
    }
    
    // Check all columns
    for (int col = 0; col < 3; col++) {
        if (buttons[col].getText() == player &&
            buttons[col + 3].getText() == player &&
            buttons[col + 6].getText() == player) {
            return true;
        }
    }
    
    // Check diagonal (top-left to bottom-right)
    if (buttons[0].getText() == player &&
        buttons[4].getText() == player &&
        buttons[8].getText() == player) {
        return true;
    }
    
    // Check diagonal (top-right to bottom-left)
    if (buttons[2].getText() == player &&
        buttons[4].getText() == player &&
        buttons[6].getText() == player) {
        return true;
    }
    
    return false;
}
