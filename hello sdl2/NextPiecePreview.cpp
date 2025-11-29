//
//  NextPiecePreview.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/11/25.
//

#include "NextPiecePreview.h"
#include "Globals.h"
#include "Colors.h"
#include "Texture.h"

NextPiecePreview::NextPiecePreview(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height) {
}

void NextPiecePreview::render(const Tetra* nextPiece) {
    SDL_Renderer* renderer = Globals::getRenderer();
    
    // Draw the box outline
    SDL_FRect box{x, y, width, height};
    SDL_SetRenderDrawColor(renderer, Colors::black.r, Colors::black.g, Colors::black.b, Colors::black.a);
    SDL_RenderRect(renderer, &box);
    
    // Draw "NEXT" label
    LTexture nextLabel;
    if (nextLabel.loadFromRenderedText("Next", Colors::black)) {
        // Center the label horizontally, position near top of box
        float labelX = x + (width - nextLabel.getWidth()) / 2.0f;
        float labelY = y + 10;
        nextLabel.render(static_cast<int>(labelX), static_cast<int>(labelY));
    }
    
    // Render the next piece if it exists
    if (nextPiece) {
        // Calculate center position for the preview piece
        float boxCenterX = x + width / 2.0f;
        float boxCenterY = y + height / 2.0f;
        
        // Offset to center the 4x4 piece grid
        float previewStartX = boxCenterX - (4 * PREVIEW_CELL_SIZE) / 2.0f;
        float previewStartY = boxCenterY - (4 * PREVIEW_CELL_SIZE) / 2.0f;
        
        nextPiece->render(renderer, PREVIEW_CELL_SIZE, previewStartX, previewStartY);
    }
}

