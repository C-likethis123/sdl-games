//
//  Texture.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 25/10/25.
//

#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL3/SDL.h>
#include <string>

class LTexture
{
    public:
        //Initializes variables
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile( const std::string& path );

        //Deallocates texture
//        void free();

        //Renders texture at given point
        void render( float x, float y );

        //Gets image dimensions
        float getWidth() const;
        float getHeight() const;

    private:
        //The actual hardware texture
        SDL_Texture* mTexture;

        //Image dimensions
        float mWidth;
        float mHeight;
};

#endif // LTEXTURE_H
