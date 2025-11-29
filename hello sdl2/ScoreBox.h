//
//  ScoreBox.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/11/25.
//

#ifndef SCOREBOX_H
#define SCOREBOX_H

#include <SDL3/SDL.h>

class ScoreBox {
public:
    ScoreBox(float x, float y, float width, float height);
    
    void render(int score);
    
    // Setters for position/size if needed
    void setPosition(float x, float y) { this->x = x; this->y = y; }
    void setSize(float width, float height) { this->width = width; this->height = height; }
    
private:
    float x;
    float y;
    float width;
    float height;
};

#endif // SCOREBOX_H

