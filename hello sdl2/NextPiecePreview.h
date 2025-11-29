//
//  NextPiecePreview.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/11/25.
//

#ifndef NEXTPIECEPREVIEW_H
#define NEXTPIECEPREVIEW_H

#include <SDL3/SDL.h>
#include "Tetra.h"

class NextPiecePreview {
public:
    NextPiecePreview(float x, float y, float width, float height);
    
    void render(const Tetra* nextPiece);
    
    // Setters for position/size if needed
    void setPosition(float x, float y) { this->x = x; this->y = y; }
    void setSize(float width, float height) { this->width = width; this->height = height; }
    
private:
    float x;
    float y;
    float width;
    float height;
    
    static constexpr float PREVIEW_CELL_SIZE = 20.0f;
};

#endif // NEXTPIECEPREVIEW_H

