//
//  Colors.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 28/11/25.
//

#ifndef COLORS_H
#define COLORS_H

#include <SDL3/SDL.h>

// Color constants namespace
namespace Colors {
    // Basic colors
    constexpr SDL_Color black       = {0, 0, 0, 255};
    constexpr SDL_Color white       = {255, 255, 255, 255};
    constexpr SDL_Color red         = {255, 0, 0, 255};
    constexpr SDL_Color green       = {0, 255, 0, 255};
    constexpr SDL_Color blue        = {0, 0, 255, 255};
    constexpr SDL_Color yellow      = {255, 255, 0, 255};
    constexpr SDL_Color cyan        = {0, 255, 255, 255};
    constexpr SDL_Color magenta     = {255, 0, 255, 255};
    
    // Grays
    constexpr SDL_Color gray        = {128, 128, 128, 255};
    constexpr SDL_Color lightGray   = {200, 200, 200, 255};
    constexpr SDL_Color darkGray    = {64, 64, 64, 255};
    
    // Additional useful colors
    constexpr SDL_Color orange      = {255, 165, 0, 255};
    constexpr SDL_Color purple      = {128, 0, 128, 255};
    constexpr SDL_Color pink        = {255, 192, 203, 255};
    constexpr SDL_Color brown       = {165, 42, 42, 255};
    
    // UI colors
    constexpr SDL_Color hoverBlue   = {173, 216, 230, 255};  // Light blue for hover effects
    constexpr SDL_Color gridDark    = {30, 30, 30, 255};     // Dark background for grids
    constexpr SDL_Color gridLine    = {50, 50, 50, 255};     // Grid line color
    
    // Transparent
    constexpr SDL_Color transparent = {0, 0, 0, 0};
}

#endif // COLORS_H

