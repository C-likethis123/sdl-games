//
//  Dot.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 26/10/25.
//

#ifndef DOT_H
#define DOT_H

#include <SDL3/SDL.h>
#include "Texture.h"

class Dot {
public:
        //The dimensions of the dot
        static const int DOT_WIDTH = 20;
        static const int DOT_HEIGHT = 20;

        //Maximum axis velocity of the dot
        static const int DOT_VEL = 10;

        //Initializes the variables
        Dot();

        //Takes key presses and adjusts the dot's velocity
        void handleEvent( SDL_Event& e );

        //Moves the dot
        void move();

        //Shows the dot on the screen
        void render();

    private:
        //The X and Y offsets of the dot
        int mPosX{0};
        int mPosY{0};

        //The velocity of the dot
        int mVelX{0};
        int mVelY{0};
    
        LTexture dotTexture;
};

#endif // DOT_H
