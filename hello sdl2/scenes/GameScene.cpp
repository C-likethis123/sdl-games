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

GameScene::GameScene() : lastMoveTime(0), isClearing(false), clearStartTime(0), blinkCount(0) {
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

void GameScene::reset() {
    // Clear the grid
    for (int row = 0; row < TETRIS_GRID_HEIGHT; row++) {
        for (int col = 0; col < TETRIS_GRID_WIDTH; col++) {
            tetrisGrid[row][col] = 0;
        }
    }
    
    // Reset game state
    isClearing = false;
    linesToClear.clear();
    
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
        // Move next piece to current piece position
        int startX = TETRIS_GRID_WIDTH / 2 - 2;
        int startY = 0;
        
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
        if (y >= 0 && y < TETRIS_GRID_HEIGHT && x >= 0 && x < TETRIS_GRID_WIDTH) {
            if (tetrisGrid[y][x] == 1) {
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
            case SDLK_SPACE:
                // Rotate clockwise
                rotateIfValid(true);
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
    
    // Check for complete lines
    checkAndClearLines();
}

void GameScene::checkAndClearLines() {
    linesToClear = findCompleteLines();
    
    if (!linesToClear.empty()) {
        // Start line clearing animation
        isClearing = true;
        clearStartTime = SDL_GetTicks();
        blinkCount = 0;
    } else {
        // No lines to clear, spawn new piece immediately
        spawnNewPiece();
    }
}

std::vector<int> GameScene::findCompleteLines() {
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

void GameScene::removeLines(const std::vector<int>& lines) {
    // Remove lines from bottom to top to avoid index issues
    for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
        int lineToRemove = *it;
        
        // Remove the line by shifting everything above it down
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
    
    // Add score (10 points per line)
    Globals::setScore(Globals::getScore() + lines.size() * 10);
    
    // Clear animation state and spawn new piece
    isClearing = false;
    linesToClear.clear();
    spawnNewPiece();
}

void GameScene::updateLineClearAnimation() {
    if (!isClearing) return;
    
    uint64_t currentTime = SDL_GetTicks();
    uint64_t elapsed = currentTime - clearStartTime;
    
    // Check if enough time has passed for next blink
    int currentBlinkPhase = elapsed / BLINK_INTERVAL_MS;
    
    if (currentBlinkPhase >= TOTAL_BLINKS) {
        // Animation complete, remove lines
        removeLines(linesToClear);
    }
}

void GameScene::updateGravity() {
    if (isClearing) return;
    
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
                    if (y >= TETRIS_GRID_HEIGHT) {
                        canMoveDown = false;
                        break;
                    }
                    
                    // Check collision with locked pieces
                    if (y >= 0 && x >= 0 && x < TETRIS_GRID_WIDTH && tetrisGrid[y][x] == 1) {
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
    if (!currentPiece || isClearing) return;
    
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
                float x = TETRIS_GRID_START_X + (currentPiece->getX() + col) * TETRIS_CELL_SIZE;
                float y = TETRIS_GRID_START_Y + (ghostY + row) * TETRIS_CELL_SIZE;
                
                // Draw ghost piece
                SDL_FRect rect{x + 2, y + 2, TETRIS_CELL_SIZE - 4, TETRIS_CELL_SIZE - 4};
                SDL_RenderRect(renderer, &rect);
            }
        }
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
    renderTetrisGrid();
    
    // Render ghost piece (before rendering current piece so it appears behind)
    renderGhostPiece();
    
    // Render current piece (only if not clearing lines)
    if (currentPiece && !isClearing) {
        currentPiece->render(Globals::getRenderer(), TETRIS_CELL_SIZE, 
                            TETRIS_GRID_START_X, TETRIS_GRID_START_Y);
    }
}
