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
#include <string_view>
#include <memory>


namespace {    
    // Color key for transparency (cyan: R=0, G=255, B=255)
    constexpr Uint8 COLOR_KEY_R = 0x00;
    constexpr Uint8 COLOR_KEY_G = 0xFF;
    constexpr Uint8 COLOR_KEY_B = 0xFF;
}
class LTexture
{
    public:
        //Initializes variables
        LTexture();

        //Deallocates memory
        ~LTexture() = default;

        // Delete copy operations (textures shouldn't be copied)
        LTexture(const LTexture&) = delete;
        LTexture& operator=(const LTexture&) = delete;

        // Default move operations (allow transfer of ownership)
        LTexture(LTexture&&) noexcept = default;
        LTexture& operator=(LTexture&&) noexcept = default;

        // Loads image at specified path
        // Returns true on success, false on failure (with error logged)
    [[nodiscard]] bool loadFromFile(const std::string& path, int red = COLOR_KEY_R, int green = COLOR_KEY_G, int blue = COLOR_KEY_B);

        // Deallocates texture and resets dimensions
        void free() noexcept;

        // Renders texture at given point
        // If clip is provided, only that portion of the texture is rendered
        void render(float x, float y, const SDL_FRect* clip = nullptr, double angle = 0.0, SDL_FPoint* center = nullptr, SDL_FlipMode flip = SDL_FLIP_NONE) const;

        // Gets image dimensions
        [[nodiscard]] float getWidth() const noexcept;
        [[nodiscard]] float getHeight() const noexcept;

        // Check if texture is loaded
        [[nodiscard]] explicit operator bool() const noexcept { return mTexture != nullptr; }
        
        // Check if texture is loaded (alternative to operator bool)
        [[nodiscard]] bool isLoaded() const noexcept { return mTexture != nullptr; }

    private:
        // Custom deleter for SDL_Texture
        struct SDL_TextureDeleter {
            void operator()(SDL_Texture* texture) const {
                if (texture) {
                    SDL_DestroyTexture(texture);
                }
            }
        };

        //The actual hardware texture
        // best practice: smart pointers
        std::unique_ptr<SDL_Texture, SDL_TextureDeleter> mTexture;

        //Image dimensions
        float mWidth{0.0f};
        float mHeight{0.0f};
};

#endif // LTEXTURE_H
