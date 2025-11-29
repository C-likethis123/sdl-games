//
//  GameScene.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 24/11/25.
//

#include "GameScene.h"
#include "../Globals.h"
#include "../Colors.h"
#include <format>
#include <random>
#include <sstream>

GameScene::GameScene() : lastMoveTime(0) {
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
    // Reset game state TODO reset the grid
    
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
            Globals::setScene("gameover");
            reset();
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
    int targetY = calculateGhostPieceY();
    
    // Move piece directly to the target position
    while (currentPiece->getY() < targetY) {
        currentPiece->moveDown();
    }
    
    // Lock the piece immediately
    tetrisGrid.lockPiece(*currentPiece);
    
    // Check for lines and clear them
    auto lines = tetrisGrid.findCompleteLines();
    if (!lines.empty()) {
        removeLines(lines);
    } else {
        spawnNewPiece();
    }
    
    // Reset gravity timer
    lastMoveTime = SDL_GetTicks();
}

//void GameScene::lockPiece() {
//    if (!currentPiece) return;
//    
//    // Lock the piece into the grid
//    auto cells = currentPiece->getOccupiedCells();
//    for (const auto& [x, y] : cells) {
//        if (y >= 0 && y < TETRIS_GRID_HEIGHT && x >= 0 && x < TETRIS_GRID_WIDTH) {
//            tetrisGrid[y][x] = 1;
//        }
//    }
//    
//    // Check for complete lines
//    checkAndClearLines();
//}

//void GameScene::checkAndClearLines() {
//    linesToClear = findCompleteLines();
//    
//    if (!linesToClear.empty()) {
//        // Start line clearing animation
//        isClearing = true;
//        clearStartTime = SDL_GetTicks();
//        blinkCount = 0;
//    } else {
//        // No lines to clear, spawn new piece immediately
//        spawnNewPiece();
//    }
//}

// findCompleteLines() moved to TetrisGrid

void GameScene::removeLines(const std::vector<int>& lines) {
    // TODO: This should be refactored to use tetrisGrid.removeLines()
    // For now, delegate to TetrisGrid
    tetrisGrid.removeLines(lines);
    
    // Add score (10 points per line)
    Globals::setScore(Globals::getScore() + lines.size() * 10);
    
    // Spawn new piece
    spawnNewPiece();
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
    // TODO: isClearing logic needs to be refactored
    // if (isClearing) return;
    
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
                removeLines(lines);
            } else {
                spawnNewPiece();
            }
        }
        lastMoveTime = currentTime;
    }
}

int GameScene::calculateGhostPieceY() {
    if (!currentPiece) return 0;
    
    int ghostY = currentPiece->getY();
    const auto& shape = currentPiece->getShape();
    int currentX = currentPiece->getX();
    
    // Keep moving down until we can't anymore
    while (true) {
        bool canMoveDown = true;
        
        // Check each cell of the piece at the next position
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                if (shape[row][col] == 1) {
                    int x = currentX + col;
                    int y = ghostY + row + 1;  // Check one position down
                    
                    // Check boundaries
                    if (y >= tetrisGrid.getHeight()) {
                        canMoveDown = false;
                        break;
                    }
                    
                    // Check collision with locked pieces
                    if (y >= 0 && x >= 0 && x < tetrisGrid.getWidth() && tetrisGrid.isCellOccupied(x, y)) {
                        canMoveDown = false;
                        break;
                    }
                }
            }
            if (!canMoveDown) break;
        }
        
        if (!canMoveDown) {
            break;
        }
        
        ghostY++;
    }
    
    return ghostY;
}

void GameScene::renderGhostPiece() {
    if (!currentPiece) return;
    
    SDL_Renderer* renderer = Globals::getRenderer();
    int ghostY = calculateGhostPieceY();
    
    // Only render if ghost piece is different from current piece position
    if (ghostY == currentPiece->getY()) {
        return;
    }
    
    // Get the current piece's shape and color
    const auto& shape = currentPiece->getShape();
    SDL_Color color = currentPiece->getColor();
    
    // Render ghost piece as outlined rectangles (not filled)
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 128);  // Semi-transparent
    
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (shape[row][col] == 1) {
                float x = tetrisGrid.getStartX() + (currentPiece->getX() + col) * tetrisGrid.getCellSize();
                float y = tetrisGrid.getStartY() + (ghostY + row) * tetrisGrid.getCellSize();
                
                // Draw ghost piece
                SDL_FRect rect{x + 2, y + 2, tetrisGrid.getCellSize() - 4, tetrisGrid.getCellSize() - 4};
                SDL_RenderRect(renderer, &rect);
            }
        }
    }
}

void GameScene::renderScoreBox() {
    SDL_Renderer* renderer = Globals::getRenderer();
    SDL_FRect score_box(50, 300, 200, 150);
    SDL_SetRenderDrawColor(renderer, Colors::black.r, Colors::black.g, Colors::black.b, Colors::black.a);
    SDL_RenderRect(renderer, &score_box);
    LTexture scoreLabel;
    if (scoreLabel.loadFromRenderedText("Score", Colors::black)) {
        scoreLabel.render(100, 320);
    }
    LTexture scoreValue;
    if (scoreValue.loadFromRenderedText(std::format("{}",Globals::getScore()), Colors::black)) {
        scoreValue.render(100, 350);
    }
}

void GameScene::renderNextPiecePreview() {
    SDL_Renderer* renderer = Globals::getRenderer();
 
    SDL_FRect next_box(50, 50, 200, 150);
    SDL_SetRenderDrawColor(renderer, Colors::black.r, Colors::black.g, Colors::black.b, Colors::black.a);
    SDL_RenderRect(renderer, &next_box);
    
    // Draw "NEXT" label
    LTexture nextLabel;
    if (nextLabel.loadFromRenderedText("Next", Colors::black)) {
        nextLabel.render(140 - nextLabel.getWidth() / 2, 60);
    }
    
    if (nextPiece) {
        // Calculate center position for the preview piece
        // Preview cell size (larger for visibility)
        float previewCellSize = 20.0f;
        
        // Center the piece in the box
        // Box center: 150 (50 + 200/2), 125 (50 + 150/2)
        float boxCenterX = 150.0f;
        float boxCenterY = 125.0f;
        
        // Offset to center the 4x4 piece grid
        float previewStartX = boxCenterX - (4 * previewCellSize) / 2.0f;
        float previewStartY = boxCenterY - (4 * previewCellSize) / 2.0f;
        
        nextPiece->render(renderer, previewCellSize, previewStartX, previewStartY);
    }
}

void GameScene::render() {
    // Update line clearing animation
    updateLineClearAnimation();
    
    // Update gravity (only if not clearing)
    updateGravity();
    
    // Render next piece preview box
    renderNextPiecePreview();
    
    // Render score box
    renderScoreBox();
       
    // Render Tetris grid
    tetrisGrid.render();
    
    // Render ghost piece (before rendering current piece so it appears behind)
    renderGhostPiece();
    
    // Render current piece
    if (currentPiece) {
        currentPiece->render(Globals::getRenderer(), tetrisGrid.getCellSize(), 
                            tetrisGrid.getStartX(), tetrisGrid.getStartY());
    }
}
