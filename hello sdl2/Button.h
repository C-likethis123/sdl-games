//
//  Button.h
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/10/25.
//

#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <functional>
#include <SDL3/SDL.h>
#include "Texture.h"

class Button {
public:
    Button(const std::string& text, std::function<void()> func);
    
    // Delete copy operations (Button contains non-copyable LTexture)
    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;
    
    // Default move operations (allow Button to be moved)
    Button(Button&&) noexcept = default;
    Button& operator=(Button&&) noexcept = default;
    
    ~Button() = default;
    
    void onClick();
    bool isClicked(SDL_Event& e);
    void render();
private:
    void initialise();
    std::string text;
    int width{30};
    int height{30};
    std::function<void()> func;
    LTexture textTexture;
    
    // area that the button is being rendered on
    SDL_FRect rect{160, 340, 30, 30};
};

#endif
