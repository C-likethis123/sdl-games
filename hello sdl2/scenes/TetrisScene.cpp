//
//  TetrisScene.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 24/11/25.
//

#include "TetrisScene.h"
#include "../Globals.h"
#include "../Colors.h"
#include <random>

TetrisScene::TetrisScene() {
    // Initialize grid with all empty cells
    grid.resize(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 0));
}

void TetrisScene::initialise() {
    spawnNewPiece();
}

void TetrisScene::spawnNewPiece() {
    // Randomly select a piece type
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 6);
    
    TetraType type = static_cast<TetraType>(dis(gen));
    
    // Spawn at top center of grid
    int startX = GRID_WIDTH / 2 - 2;
    int startY = 0;
    
    currentPiece = std::make_unique<Tetra>(type, startX, startY);
}

void TetrisScene::handleEvent(SDL_Event& e) {
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

bool TetrisScene::canMove(int deltaX, int deltaY) {
    if (!currentPiece) return false;
    
    // Get occupied cells with new position
    auto cells = currentPiece->getOccupiedCells();
    
    for (const auto& [x, y] : cells) {
        int newX = x + deltaX;
        int newY = y + deltaY;
        
        // Check boundaries
        if (newX < 0 || newX >= GRID_WIDTH || newY >= GRID_HEIGHT) {
            return false;
        }
        
        // Check collision with locked pieces (skip if newY is negative, piece is still spawning)
        if (newY >= 0 && grid[newY][newX] == 1) {
            return false;
        }
    }
    
    return true;
}

bool TetrisScene::canRotate(bool clockwise) {
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
        if (x < 0 || x >= GRID_WIDTH || y >= GRID_HEIGHT) {
            valid = false;
            break;
        }
        
        // Check collision with locked pieces (skip if y is negative)
        if (y >= 0 && grid[y][x] == 1) {
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

void TetrisScene::lockPiece() {
    if (!currentPiece) return;
    
    // Lock the piece into the grid
    auto cells = currentPiece->getOccupiedCells();
    for (const auto& [x, y] : cells) {
        if (y >= 0 && y < GRID_HEIGHT && x >= 0 && x < GRID_WIDTH) {
            grid[y][x] = 1;
        }
    }
    
    // Spawn new piece
    spawnNewPiece();
}

void TetrisScene::renderGrid() {
    SDL_Renderer* renderer = Globals::getRenderer();
    
    // Draw grid background
    SDL_SetRenderDrawColor(renderer, Colors::gridDark.r, Colors::gridDark.g, Colors::gridDark.b, Colors::gridDark.a);
    SDL_FRect gridRect = {GRID_START_X, GRID_START_Y, 
                          GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE};
    SDL_RenderFillRect(renderer, &gridRect);
    
    // Draw grid lines
    SDL_SetRenderDrawColor(renderer, Colors::gridLine.r, Colors::gridLine.g, Colors::gridLine.b, Colors::gridLine.a);
    
    // Vertical lines
    for (int i = 0; i <= GRID_WIDTH; i++) {
        float x = GRID_START_X + i * CELL_SIZE;
        SDL_RenderLine(renderer, x, GRID_START_Y, 
                      x, GRID_START_Y + GRID_HEIGHT * CELL_SIZE);
    }
    
    // Horizontal lines
    for (int i = 0; i <= GRID_HEIGHT; i++) {
        float y = GRID_START_Y + i * CELL_SIZE;
        SDL_RenderLine(renderer, GRID_START_X, y, 
                      GRID_START_X + GRID_WIDTH * CELL_SIZE, y);
    }
    
    // Draw locked pieces
    SDL_SetRenderDrawColor(renderer, Colors::lightGray.r, Colors::lightGray.g, Colors::lightGray.b, Colors::lightGray.a);
    for (int row = 0; row < GRID_HEIGHT; row++) {
        for (int col = 0; col < GRID_WIDTH; col++) {
            if (grid[row][col] == 1) {
                SDL_FRect cellRect = {
                    GRID_START_X + col * CELL_SIZE,
                    GRID_START_Y + row * CELL_SIZE,
                    CELL_SIZE,
                    CELL_SIZE
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

void TetrisScene::render() {
    renderGrid();
    
    // Render current piece
    if (currentPiece) {
        currentPiece->render(Globals::getRenderer(), CELL_SIZE, GRID_START_X, GRID_START_Y);
    }
}

