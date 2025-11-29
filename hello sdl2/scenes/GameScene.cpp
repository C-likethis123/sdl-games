//
//  GameScene.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 24/11/25.
//

#include "GameScene.h"
#include "../Globals.h"
#include <random>

GameScene::GameScene() 
    : lastMoveTime(0), 
      scoreBox(50, 300, 200, 150),
      nextPiecePreview(50, 50, 200, 150) {
    // Initialize Tetris grid with all empty cells
    initialise();
}

void GameScene::initialise() {
    // Generate first piece and next piece
    nextPiece = std::make_unique<Tetra>(getRandomTetraType(), 0, 0);
    spawnNewPiece();
    lastMoveTime = SDL_GetTicks();
}

void GameScene::reset() {
    // Clear the grid
    tetrisGrid.clear();
    
    // Reset score
    Globals::setScore(0);
    
    // Restart game
    initialise();
}

TetraType GameScene::getRandomTetraType() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 6);
    
    return static_cast<TetraType>(dis(gen));
}

void GameScene::spawnNewPiece() {
    if (nextPiece) {
        // Get spawn position from the grid
        auto [startX, startY] = tetrisGrid.getSpawnPosition();
        
        // Create piece at spawn position
        currentPiece = std::make_unique<Tetra>(nextPiece->getType(), startX, startY);
        
        // Check if the newly spawned piece collides with existing pieces (game over)
        if (checkGameOver()) {
            // Clear the game state before transitioning
            currentPiece.reset();
            nextPiece.reset();

            // Clear the grid
            tetrisGrid.clear();
            
            // Transition to game over scene
            Globals::setScene("gameover");
            return;
        }
    }
    
    // Generate new next piece (positioned at 0,0 for preview, will be repositioned when spawned)
    nextPiece = std::make_unique<Tetra>(getRandomTetraType(), 0, 0);
}

bool GameScene::checkGameOver() {
    if (!currentPiece) return false;
    
    auto cells = currentPiece->getOccupiedCells();

    for (const auto& [x, y] : cells) {
        // Check if any cell of the new piece overlaps with locked pieces
        if (y >= 0 && y < tetrisGrid.getHeight() && x >= 0 && x < tetrisGrid.getWidth()) {
            if (tetrisGrid.isCellOccupied(x, y)) {
                return true;  // Game over!
            }
        }
    }
    
    return false;
}

void GameScene::handleEvent(SDL_Event& e) {
    // Call parent's handleEvent for button handling
    Scene::handleEvent(e);
    
    // Handle keyboard input
    if (e.type == SDL_EVENT_KEY_DOWN) {
        switch (e.key.key) {
            case SDLK_LEFT:
                if (canMove(-1, 0)) {
                    currentPiece->moveLeft();
                }
                break;
                
            case SDLK_RIGHT:
                if (canMove(1, 0)) {
                    currentPiece->moveRight();
                }
                break;
                
            case SDLK_DOWN:
                if (canMove(0, 1)) {
                    currentPiece->moveDown();
                    lastMoveTime = SDL_GetTicks();  // Reset gravity timer
                }
                break;
                
            case SDLK_UP:
                // Rotate clockwise
                rotateIfValid(true);
                break;
                
            case SDLK_SPACE:
                hardDrop();
                break;
                
            case SDLK_Z:
                // Rotate counter-clockwise
                rotateIfValid(false);
                break;
        }
    }
}

bool GameScene::canMove(int deltaX, int deltaY) {
     if (!currentPiece) return false;
    
    // Get occupied cells with new position
    auto cells = currentPiece->getOccupiedCells();
    
    for (const auto& [x, y] : cells) {
        int newX = x + deltaX;
        int newY = y + deltaY;
        
        // Check boundaries
        if (newX < 0 || newX >= tetrisGrid.getWidth() || newY < 0 || newY >= tetrisGrid.getHeight()) {
            return false;
        }
        
        // Check collision with locked pieces (skip if newY is negative, piece is still spawning)
        if (newY >= 0 && tetrisGrid.isCellOccupied(newX, newY)) {
            return false;
        }
    }
    
    return true;
}

bool GameScene::rotateIfValid(bool clockwise) {
    if (!currentPiece) return false;
    
    // Temporarily rotate to check
    if (clockwise) {
        currentPiece->rotateClockwise();
    } else {
        currentPiece->rotateCounterClockwise();
    }
    
    // Check if rotated position is valid
    bool valid = true;
    auto cells = currentPiece->getOccupiedCells();
    
    for (const auto& [x, y] : cells) {
        // Check boundaries
        if (x < 0 || x >= tetrisGrid.getWidth() || y >= tetrisGrid.getHeight()) {
            valid = false;
            break;
        }
        
        // Check collision with locked pieces (skip if y is negative)
        if (y >= 0 && tetrisGrid.isCellOccupied(x, y)) {
            valid = false;
            break;
        }
    }
    
    // Rotate back if invalid
    if (!valid) {
        if (clockwise) {
            currentPiece->rotateCounterClockwise();
        } else {
            currentPiece->rotateClockwise();
        }
    }
    
    return valid;
}

void GameScene::hardDrop() {
    if (!currentPiece) return;
    
    // Calculate where the piece will land
    int targetY = ghostPieceRenderer.calculateDropPosition(currentPiece.get(), tetrisGrid);
    
    // Move piece directly to the target position
    while (currentPiece->getY() < targetY) {
        currentPiece->moveDown();
    }
    
    // Lock the piece immediately
    tetrisGrid.lockPiece(*currentPiece);
    
    // Check for lines and clear them
    auto lines = tetrisGrid.findCompleteLines();
    if (!lines.empty()) {
        tetrisGrid.removeLines(lines);
        Globals::setScore(Globals::getScore() + lines.size() * 10);
    }
    spawnNewPiece();

    // Reset gravity timer
    lastMoveTime = SDL_GetTicks();
}

void GameScene::updateLineClearAnimation() {
    // TODO: Animation logic needs to be properly refactored
    // Commenting out for now as variables moved to TetrisGrid
//    if (!isClearing) return;
//    
//    uint64_t currentTime = SDL_GetTicks();
//    uint64_t elapsed = currentTime - clearStartTime;
//    
//    // Check if enough time has passed for next blink
//    int currentBlinkPhase = elapsed / BLINK_INTERVAL_MS;
//    
//    if (currentBlinkPhase >= TOTAL_BLINKS) {
//        // Animation complete, remove lines
//        removeLines(linesToClear);
//    }
}

void GameScene::updateGravity() {
    // Don't update if no current piece (e.g., during game over)
    if (!currentPiece) return;
    
    uint64_t currentTime = SDL_GetTicks();
    
    if (currentTime - lastMoveTime >= MOVE_DELAY_MS) {
        if (canMove(0, 1)) {
            currentPiece->moveDown();
        } else {
            // Can't move down, lock the piece
            tetrisGrid.lockPiece(*currentPiece);
            
            // Check for lines and clear them
            auto lines = tetrisGrid.findCompleteLines();
            if (!lines.empty()) {
                tetrisGrid.removeLines(lines);
                Globals::setScore(Globals::getScore() + lines.size() * 10);
            }
            spawnNewPiece();
        }
        lastMoveTime = currentTime;
    }
}

// calculateGhostPieceY() and renderGhostPiece() moved to GhostPieceRenderer component

// renderScoreBox() moved to ScoreBox component

// renderNextPiecePreview() moved to NextPiecePreview component

void GameScene::render() {
    // Update line clearing animation
    updateLineClearAnimation();
    
    // Update gravity (only if not clearing)
    updateGravity();
    
    // Render next piece preview box
    nextPiecePreview.render(nextPiece.get());
    
    // Render score box
    scoreBox.render(Globals::getScore());
       
    // Render Tetris grid
    tetrisGrid.render();
    
    // Render ghost piece (before rendering current piece so it appears behind)
    ghostPieceRenderer.render(currentPiece.get(), tetrisGrid);
    
    // Render current piece
    if (currentPiece) {
        currentPiece->render(Globals::getRenderer(), tetrisGrid.getCellSize(), 
                            tetrisGrid.getStartX(), tetrisGrid.getStartY());
    }
}
