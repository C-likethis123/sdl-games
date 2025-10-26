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

namespace {
    // Custom deleter for SDL_Surface RAII
    struct SDL_SurfaceDeleter {
        void operator()(SDL_Surface* surface) const {
            if (surface) {
                SDL_DestroySurface(surface);
            }
        }
    };
    
    // Color key for transparency (cyan: R=0, G=255, B=255)
    constexpr Uint8 COLOR_KEY_R = 0x00;
    constexpr Uint8 COLOR_KEY_G = 0xFF;
    constexpr Uint8 COLOR_KEY_B = 0xFF;
}

LTexture::LTexture() = default;

bool LTexture::loadFromFile(const std::string& file_name) {
    // Free existing texture first to prevent memory leak
    free();
    
    std::filesystem::path path = getResourcePath(file_name);
    const std::string& file_extension = path.extension();
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Loading image at: %s\n", path.c_str());

    std::unique_ptr<SDL_Surface, SDL_SurfaceDeleter> loadedSurface;
    
    if (file_extension.ends_with(".bmp")) {
        loadedSurface.reset(SDL_LoadBMP(path.c_str()));
    } else if (file_extension.ends_with(".png")) {
        loadedSurface.reset(IMG_Load(path.c_str()));
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Unsupported file format: %s (file: %s)", 
                     file_extension.c_str(), path.c_str());
        return false;
    }
    
    if (!loadedSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed to load image: %s. SDL Error: %s", 
                     path.c_str(), SDL_GetError());
        return false;
    }

    // Color key image - specify a color, all pixels of that color will be treated as transparent
    // SDL_MapSurfaceRGB creates a pixel value from RGB components
    SDL_SetSurfaceColorKey(loadedSurface.get(), true, 
                          SDL_MapSurfaceRGB(loadedSurface.get(), COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
    
    // Create texture from surface
    SDL_Renderer* renderer = Globals::getRenderer();
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Renderer is null, cannot create texture");
        return false;
    }
    
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface.get());
    if (!newTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, 
                    "Unable to create texture from %s! SDL Error: %s", 
                    path.c_str(), SDL_GetError());
        return false;
    }
    
    // Get texture dimensions and transfer ownership
    if (!SDL_GetTextureSize(newTexture, &mWidth, &mHeight)) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, 
                    "Failed to get texture size for %s! SDL Error: %s", 
                    path.c_str(), SDL_GetError());
        SDL_DestroyTexture(newTexture);
        return false;
    }
    
    mTexture.reset(newTexture);
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, 
                "Successfully loaded texture: %s (%.0fx%.0f)", 
                path.c_str(), mWidth, mHeight);
    
    return true;
}

void LTexture::render(float x, float y, const SDL_FRect* clip) const
{
    if (!mTexture) {
        SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Attempted to render null texture");
        return;
    }
    
    SDL_Renderer* renderer = Globals::getRenderer();
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Renderer is null, cannot render texture");
        return;
    }
    
    // Set rendering space and render to screen
    SDL_FRect renderQuad = {x, y, mWidth, mHeight};
    
    // If a clip rectangle is provided, adjust the rendering dimensions
    if (clip != nullptr) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    
    if (!SDL_RenderTexture(renderer, mTexture.get(), clip, &renderQuad)) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, 
                    "Failed to render texture! SDL Error: %s", SDL_GetError());
    }
}

float LTexture::getWidth() const noexcept
{
    return mWidth;
}

float LTexture::getHeight() const noexcept
{
    return mHeight;
}

void LTexture::free() noexcept
{
    mTexture.reset();  // Automatically calls deleter if texture exists
    mWidth = 0.0f;
    mHeight = 0.0f;
}
