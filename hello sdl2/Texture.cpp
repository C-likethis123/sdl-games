//
//  Texture.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 25/10/25.
//
#include "Texture.h"
#include "globals.h"
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <filesystem>


LTexture::LTexture() : mTexture(NULL), mWidth(0), mHeight(0) {}

bool LTexture::loadFromFile(const std::string& file_name) {
    SDL_Texture* newTexture = NULL;
    std::filesystem::path path = getResourcePath(file_name);
    const std::string file_extension = path.extension();
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Loading image at: %s\n", path.c_str());

    SDL_Surface* loadedSurface = NULL;
    if (file_extension.find(".bmp") != std::string::npos) {
        loadedSurface = SDL_LoadBMP(path.c_str());
    } else if (file_extension.find(".png") != std::string::npos) {
        loadedSurface = IMG_Load(path.c_str());
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "path: %s is invalid", path.c_str());
    }

    //Color key image - this makes it transparent
    SDL_SetSurfaceColorKey( loadedSurface, true, SDL_MapSurfaceRGB( loadedSurface, 0, 0xFF, 0xFF ) );
    // Create texture from surface
    newTexture = SDL_CreateTextureFromSurface(Globals::getRenderer(), loadedSurface);
    if (!newTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Unable to create texture! SDL Error: %s\n", SDL_GetError());
    } else {
        SDL_GetTextureSize(newTexture, &mWidth, &mHeight);
    }
    SDL_DestroySurface(loadedSurface);

    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::render( float x, float y )
{
    //Set rendering space and render to screen
    SDL_FRect renderQuad = { x, y, mWidth, mHeight };
    SDL_RenderTexture( Globals::getInstance().getRenderer(), mTexture, NULL, &renderQuad );
}

float LTexture::getWidth() const
{
    return mWidth;
}

float LTexture::getHeight() const
{
    return mHeight;
}

LTexture::~LTexture()
{
//    free();
}
