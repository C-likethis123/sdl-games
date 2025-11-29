//
//  GhostPieceRenderer.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/11/25.
//

#include "GhostPieceRenderer.h"
#include "Globals.h"

GhostPieceRenderer::GhostPieceRenderer() {
}

int GhostPieceRenderer::calculateDropPosition(const Tetra* piece, const TetrisGrid& grid) const {
    if (!piece) return 0;
    
    int ghostY = piece->getY();
    const auto& shape = piece->getShape();
    int currentX = piece->getX();
    
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
                    if (y >= grid.getHeight()) {
                        canMoveDown = false;
                        break;
                    }
                    
                    // Check collision with locked pieces
                    if (y >= 0 && x >= 0 && x < grid.getWidth() && grid.isCellOccupied(x, y)) {
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

void GhostPieceRenderer::render(const Tetra* currentPiece, const TetrisGrid& grid) {
    if (!currentPiece) return;
    
    SDL_Renderer* renderer = Globals::getRenderer();
    int ghostY = calculateDropPosition(currentPiece, grid);
    
    // Only render if ghost piece is different from current piece position
    if (ghostY == currentPiece->getY()) {
        return;
    }
    
    // Get the current piece's shape and color
    const auto& shape = currentPiece->getShape();
    SDL_Color color = currentPiece->getColor();
    
    // Render ghost piece as outlined rectangles (not filled)
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, GHOST_ALPHA);  // Semi-transparent
    
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (shape[row][col] == 1) {
                float x = grid.getStartX() + (currentPiece->getX() + col) * grid.getCellSize();
                float y = grid.getStartY() + (ghostY + row) * grid.getCellSize();
                
                // Draw ghost piece outline
                SDL_FRect rect{
                    x + GHOST_BORDER_OFFSET, 
                    y + GHOST_BORDER_OFFSET, 
                    grid.getCellSize() - GHOST_BORDER_OFFSET * 2, 
                    grid.getCellSize() - GHOST_BORDER_OFFSET * 2
                };
                SDL_RenderRect(renderer, &rect);
            }
        }
    }
}

