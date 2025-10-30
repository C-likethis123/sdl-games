//
//  Button.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/10/25.
//

#include "Button.h"
#include <SDL3/SDL_main.h>

void Button::onClick() {
    func();
}

bool Button::isClicked(SDL_Event& e) {
    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.clicks == 1) {
        return rect.x <= e.button.x && e.button.x <= rect.x + rect.w && rect.y <= e.button.y && e.button.y <= rect.y + rect.h;
    }
    return false;
}

Button::Button(const std::string& text, std::function<void()> func) : text(text), func(std::move(func)) {
    initialise();
}

void Button::initialise() {
    SDL_Color textColor{ 0, 0, 0 };
    if( !textTexture.loadFromRenderedText( text.c_str(), textColor ) )
    {
        printf( "Failed to render text texture!\n" );
    }
    rect.w = textTexture.getWidth();
    rect.h = textTexture.getHeight();
}

void Button::render() {
    
    textTexture.render( rect.x,  rect.y );
}
