//
//  GhostPieceRenderer.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/11/25.
//

#ifndef GHOSTPIECERENDERER_H
#define GHOSTPIECERENDERER_H

#include <SDL3/SDL.h>
#include "Tetra.h"
#include "TetrisGrid.h"

class GhostPieceRenderer {
public:
    GhostPieceRenderer();
    
    // Render the ghost piece for the current piece
    void render(const Tetra* currentPiece, const TetrisGrid& grid);
    
    // Calculate where the piece would land (useful for hard drop)
    int calculateDropPosition(const Tetra* piece, const TetrisGrid& grid) const;
    
private:
    
    static constexpr int GHOST_ALPHA = 128;  // Semi-transparent
    static constexpr float GHOST_BORDER_OFFSET = 2.0f;  // Inset from cell edges
};

#endif // GHOSTPIECERENDERER_H

