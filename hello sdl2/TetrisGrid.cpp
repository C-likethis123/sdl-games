#include "TetrisGrid.h"
#include "Globals.h"
#include "Colors.h"

TetrisGrid::TetrisGrid() : isClearing(false), clearStartTime(0), blinkCount(0) {
    tetrisGrid.resize(TETRIS_GRID_HEIGHT, std::vector<int>(TETRIS_GRID_WIDTH, 0));
}

TetrisGrid::~TetrisGrid() {
    tetrisGrid.clear();
}

// TODO refactor state to enum
bool TetrisGrid::isCellOccupied(int x, int y) const {
    return tetrisGrid[y][x] == 1;
}

bool TetrisGrid::isCellFree(int x, int y) const {
    return tetrisGrid[y][x] == 0;
}

void TetrisGrid::lockPiece(const Tetra& piece) {
    auto cells = piece.getOccupiedCells();
    for (const auto& [x, y] : cells) {
        if (y >= 0 && y < TETRIS_GRID_HEIGHT && x >= 0 && x < TETRIS_GRID_WIDTH) {
            tetrisGrid[y][x] = 1;
        }
    }
    // there's a check and clear lines here, but I don't think it should be here?
}

std::vector<int> TetrisGrid::findCompleteLines() {
    std::vector<int> completeLines;
    for (int row = 0; row < TETRIS_GRID_HEIGHT; row++) {
        bool isComplete = true;
        for (int col = 0; col < TETRIS_GRID_WIDTH; col++) {
            if (tetrisGrid[row][col] == 0) {
                isComplete = false;
                break;
            }
        }
        if (isComplete) {
            completeLines.push_back(row);
        }
    }
    return completeLines;
}

void TetrisGrid::removeLines(const std::vector<int>& lines) {
    // Remove lines from bottom to top to avoid index issues
    for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
        int lineToRemove = *it;
        
        // Shift everything above the removed line down by one row
        for (int row = lineToRemove; row > 0; row--) {
            for (int col = 0; col < TETRIS_GRID_WIDTH; col++) {
                tetrisGrid[row][col] = tetrisGrid[row - 1][col];
            }
        }
        
        // Clear the top row
        for (int col = 0; col < TETRIS_GRID_WIDTH; col++) {
            tetrisGrid[0][col] = 0;
        }
    }
}

void TetrisGrid::render() {
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
    // Calculate blink state for clearing animation
    bool shouldShowClearing = true;
    if (isClearing) {
        uint64_t currentTime = SDL_GetTicks();
        uint64_t elapsed = currentTime - clearStartTime;
        int blinkPhase = (elapsed / BLINK_INTERVAL_MS) % 2;  // 0 or 1
        shouldShowClearing = (blinkPhase == 0);  // Show on even phases, hide on odd
    }

    SDL_SetRenderDrawColor(renderer, Colors::lightGray.r, Colors::lightGray.g, Colors::lightGray.b, Colors::lightGray.a);
    for (int row = 0; row < TETRIS_GRID_HEIGHT; row++) {
        // Check if this row is being cleared
        bool isLineClearing = false;
        if (isClearing) {
            for (int clearLine : linesToClear) {
                if (clearLine == row) {
                    isLineClearing = true;
                    break;
                }
            }
        }
        
        for (int col = 0; col < TETRIS_GRID_WIDTH; col++) {
            if (tetrisGrid[row][col] == 1) {
                // Skip rendering if this line is clearing and in "hidden" blink phase
                if (isLineClearing && !shouldShowClearing) {
                    continue;
                }
                
                SDL_FRect cellRect = {
                    TETRIS_GRID_START_X + col * TETRIS_CELL_SIZE,
                    TETRIS_GRID_START_Y + row * TETRIS_CELL_SIZE,
                    TETRIS_CELL_SIZE,
                    TETRIS_CELL_SIZE
                };
                
                // Use white color for clearing lines to highlight them
                if (isLineClearing) {
                    SDL_SetRenderDrawColor(renderer, Colors::white.r, Colors::white.g, Colors::white.b, Colors::white.a);
                } else {
                    SDL_SetRenderDrawColor(renderer, Colors::lightGray.r, Colors::lightGray.g, Colors::lightGray.b, Colors::lightGray.a);
                }
                
                SDL_RenderFillRect(renderer, &cellRect);
                
                // Draw border
                SDL_SetRenderDrawColor(renderer, Colors::black.r, Colors::black.g, Colors::black.b, Colors::black.a);
                SDL_RenderRect(renderer, &cellRect);
            }
        }
    }
}
