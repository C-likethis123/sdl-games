//
//  ScoreBox.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/11/25.
//

#include "ScoreBox.h"
#include "Globals.h"
#include "Colors.h"
#include "Texture.h"
#include <format>

ScoreBox::ScoreBox(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height) {
}

void ScoreBox::render(int score) {
    SDL_Renderer* renderer = Globals::getRenderer();
    
    // Draw the box outline
    SDL_FRect box{x, y, width, height};
    SDL_SetRenderDrawColor(renderer, Colors::black.r, Colors::black.g, Colors::black.b, Colors::black.a);
    SDL_RenderRect(renderer, &box);
    
    // Render "Score" label
    LTexture scoreLabel;
    if (scoreLabel.loadFromRenderedText("Score", Colors::black)) {
        // Center the label horizontally, position near top of box
        float labelX = x + (width - scoreLabel.getWidth()) / 2.0f;
        float labelY = y + 20;
        scoreLabel.render(static_cast<int>(labelX), static_cast<int>(labelY));
    }
    
    // Render score value
    LTexture scoreValue;
    if (scoreValue.loadFromRenderedText(std::format("{}", score), Colors::black)) {
        float valueX = x + (width - scoreValue.getWidth()) / 2.0f;
        float valueY = y + 50;
        scoreValue.render(static_cast<int>(valueX), static_cast<int>(valueY));
    }
}

