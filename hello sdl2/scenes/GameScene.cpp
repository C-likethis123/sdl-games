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
    tetrisGrid.resize(TETRIS_GRID_HEIGHT, std::vector<int>(TETRIS_GRID_WIDTH, 0));
    initialise();
}

void GameScene::initialise() {
    // Generate first piece and next piece
    nextPiece = std::make_unique<Tetra>(getRandomTetraType(), 0, 0);
    spawnNewPiece();
    lastMoveTime = SDL_GetTicks();
}

TetraType GameScene::getRandomTetraType() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 6);
    
    return static_cast<TetraType>(dis(gen));
}

void GameScene::spawnNewPiece() {
    if (nextPiece) {
        // Move next piece to current piece position
        int startX = TETRIS_GRID_WIDTH / 2 - 2;
        int startY = 0;
        
        currentPiece = std::make_unique<Tetra>(nextPiece->getType(), startX, startY);
    }
    
    // Generate new next piece (positioned at 0,0 for preview, will be repositioned when spawned)
    nextPiece = std::make_unique<Tetra>(getRandomTetraType(), 0, 0);
}

void GameScene::handleEvent(SDL_Event& e) {
    // Call parent's handleEvent for button handling
    Scene::handleEvent(e);
    
    // Handle keyboard input for tetris controls
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
            case SDLK_SPACE:
                // Rotate clockwise
                if (canRotate(true)) {
                    currentPiece->rotateClockwise();
                }
                break;
                
            case SDLK_Z:
                // Rotate counter-clockwise
                if (canRotate(false)) {
                    currentPiece->rotateCounterClockwise();
                }
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
        if (newX < 0 || newX >= TETRIS_GRID_WIDTH || newY < 0 || newY >= TETRIS_GRID_HEIGHT) {
            return false;
        }
        
        // Check collision with locked pieces (skip if newY is negative, piece is still spawning)
        if (newY >= 0 && tetrisGrid[newY][newX] == 1) {
            return false;
        }
    }
    
    return true;
}

bool GameScene::canRotate(bool clockwise) {
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
        if (x < 0 || x >= TETRIS_GRID_WIDTH || y >= TETRIS_GRID_HEIGHT) {
            valid = false;
            break;
        }
        
        // Check collision with locked pieces (skip if y is negative)
        if (y >= 0 && tetrisGrid[y][x] == 1) {
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

void GameScene::lockPiece() {
    if (!currentPiece) return;
    
    // Lock the piece into the grid
    auto cells = currentPiece->getOccupiedCells();
    for (const auto& [x, y] : cells) {
        if (y >= 0 && y < TETRIS_GRID_HEIGHT && x >= 0 && x < TETRIS_GRID_WIDTH) {
            tetrisGrid[y][x] = 1;
        }
    }
    
    // Spawn new piece
    spawnNewPiece();
}

void GameScene::updateGravity() {
    uint64_t currentTime = SDL_GetTicks();
    
    if (currentTime - lastMoveTime >= MOVE_DELAY_MS) {
        if (canMove(0, 1)) {
            currentPiece->moveDown();
        } else {
            // Can't move down, lock the piece
            lockPiece();
        }
        lastMoveTime = currentTime;
    }
}

void GameScene::renderTetrisGrid() {
    SDL_Renderer* renderer = Globals::getRenderer();
    
    // Draw grid background
    SDL_SetRenderDrawColor(renderer, Colors::gridDark.r, Colors::gridDark.g, Colors::gridDark.b, Colors::gridDark.a);
    SDL_FRect gridRect = {TETRIS_GRID_START_X, TETRIS_GRID_START_Y, 
                          TETRIS_GRID_WIDTH * TETRIS_CELL_SIZE, 
                          TETRIS_GRID_HEIGHT * TETRIS_CELL_SIZE};
    SDL_RenderFillRect(renderer, &gridRect);
    
    // Draw grid lines
    SDL_SetRenderDrawColor(renderer, Colors::gridLine.r, Colors::gridLine.g, Colors::gridLine.b, Colors::gridLine.a);
    
    // Vertical lines
    for (int i = 0; i <= TETRIS_GRID_WIDTH; i++) {
        float x = TETRIS_GRID_START_X + i * TETRIS_CELL_SIZE;
        SDL_RenderLine(renderer, x, TETRIS_GRID_START_Y, 
                      x, TETRIS_GRID_START_Y + TETRIS_GRID_HEIGHT * TETRIS_CELL_SIZE);
    }
    
    // Horizontal lines
    for (int i = 0; i <= TETRIS_GRID_HEIGHT; i++) {
        float y = TETRIS_GRID_START_Y + i * TETRIS_CELL_SIZE;
        SDL_RenderLine(renderer, TETRIS_GRID_START_X, y, 
                      TETRIS_GRID_START_X + TETRIS_GRID_WIDTH * TETRIS_CELL_SIZE, y);
    }
    
    // Draw locked pieces
    SDL_SetRenderDrawColor(renderer, Colors::lightGray.r, Colors::lightGray.g, Colors::lightGray.b, Colors::lightGray.a);
    for (int row = 0; row < TETRIS_GRID_HEIGHT; row++) {
        for (int col = 0; col < TETRIS_GRID_WIDTH; col++) {
            if (tetrisGrid[row][col] == 1) {
                SDL_FRect cellRect = {
                    TETRIS_GRID_START_X + col * TETRIS_CELL_SIZE,
                    TETRIS_GRID_START_Y + row * TETRIS_CELL_SIZE,
                    TETRIS_CELL_SIZE,
                    TETRIS_CELL_SIZE
                };
                SDL_RenderFillRect(renderer, &cellRect);
                
                // Draw border
                SDL_SetRenderDrawColor(renderer, Colors::black.r, Colors::black.g, Colors::black.b, Colors::black.a);
                SDL_RenderRect(renderer, &cellRect);
                SDL_SetRenderDrawColor(renderer, Colors::lightGray.r, Colors::lightGray.g, Colors::lightGray.b, Colors::lightGray.a);
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
    // Update gravity
    updateGravity();
    
    // Render next piece preview box
    renderNextPiecePreview();
    
    // Render score box
    renderScoreBox();
       
    // Render Tetris grid
    renderTetrisGrid();
    
    // Render current piece
    if (currentPiece) {
        currentPiece->render(Globals::getRenderer(), TETRIS_CELL_SIZE, 
                            TETRIS_GRID_START_X, TETRIS_GRID_START_Y);
    }
}
