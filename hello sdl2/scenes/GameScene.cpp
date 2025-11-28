//
//  GameScene.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 24/11/25.
//

#include "GameScene.h"
#include "../Globals.h"
#include <format>
#include <random>
#include <sstream>

GameScene::GameScene() : lastMoveTime(0) {
    // Initialize Tetris grid with all empty cells
    tetrisGrid.resize(TETRIS_GRID_HEIGHT, std::vector<int>(TETRIS_GRID_WIDTH, 0));
    initialise();
}

void GameScene::initialise() {
    spawnNewPiece();
    lastMoveTime = SDL_GetTicks();
}

void GameScene::spawnNewPiece() {
    // Randomly select a piece type
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 6);
    
    TetraType type = static_cast<TetraType>(dis(gen));
    
    // Spawn at top center of grid
    int startX = TETRIS_GRID_WIDTH / 2 - 2;
    int startY = 0;
    
    currentPiece = std::make_unique<Tetra>(type, startX, startY);
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
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_FRect gridRect = {TETRIS_GRID_START_X, TETRIS_GRID_START_Y, 
                          TETRIS_GRID_WIDTH * TETRIS_CELL_SIZE, 
                          TETRIS_GRID_HEIGHT * TETRIS_CELL_SIZE};
    SDL_RenderFillRect(renderer, &gridRect);
    
    // Draw grid lines
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    
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
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
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
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderRect(renderer, &cellRect);
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            }
        }
    }
}

void GameScene::render() {
    SDL_FRect next_box(50, 50, 200, 150);
    SDL_SetRenderDrawColor(Globals::getRenderer(), 0, 0, 0, 255);
    SDL_RenderRect(Globals::getRenderer(), &next_box);
    currentPiece->render(Globals::getRenderer(), TETRIS_CELL_SIZE,
                        50, 50);

    // Update gravity
    updateGravity();
    
    
    SDL_FRect score_box(50, 300, 200, 150);
    SDL_SetRenderDrawColor(Globals::getRenderer(), 0, 0, 0, 255);
    SDL_RenderRect(Globals::getRenderer(), &score_box);
    if (background.loadFromRenderedText(std::format("{}",Globals::getScore()), SDL_Color(0,0,0))) {
        background.render(130, 370);
    }

       
    // Render Tetris grid
    renderTetrisGrid();
    
    // Render current piece
    if (currentPiece) {
        currentPiece->render(Globals::getRenderer(), TETRIS_CELL_SIZE, 
                            TETRIS_GRID_START_X, TETRIS_GRID_START_Y);
    }
    
    // TODO: Render title
//    titleText << "Tetris - Arrow Keys: Move | Space/Up: Rotate | Z: Rotate CCW";
}
