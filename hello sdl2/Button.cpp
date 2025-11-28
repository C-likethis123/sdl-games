//
//  Button.cpp
//  hello sdl2
//
//  Created by Chow Jia Ying on 29/10/25.
//

#include "Button.h"
#include "Globals.h"
#include "Colors.h"
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

bool Button::isMouseOver(SDL_Event& e) const {
    if (e.type == SDL_EVENT_MOUSE_MOTION) {
        float mouseX = e.motion.x;
        float mouseY = e.motion.y;
        return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
                mouseY >= rect.y && mouseY <= rect.y + rect.h);
    }
    return false;
}

Button::Button(const std::string& text, SDL_FRect rect, std::function<void()> func) : text(text), func(std::move(func)), rect(rect) {
    initialise();
}

void Button::initialise() {
    if (!text.empty()) {
        if( !textTexture.loadFromRenderedText( text.c_str(), Colors::black ) )
        {
            printf( "Failed to render text texture!\n" );
        }

        rect.w = textTexture.getWidth();
        rect.h = textTexture.getHeight();
    }
}

void Button::render() {
    // Draw hover background if hovered and cell is empty
    if (isHovered && text.empty()) {
        SDL_SetRenderDrawColor(Globals::getRenderer(), Colors::hoverBlue.r, Colors::hoverBlue.g, Colors::hoverBlue.b, Colors::hoverBlue.a);
    } else {
        SDL_SetRenderDrawColor(Globals::getRenderer(), Colors::white.r, Colors::white.g, Colors::white.b, Colors::white.a);
    }
    SDL_RenderFillRect(Globals::getRenderer(), &rect);
    
    if (!text.empty()) {
        // Center text within the button rect
        int textX = rect.x + (rect.w - textTexture.getWidth()) / 2;
        int textY = rect.y + (rect.h - textTexture.getHeight()) / 2;
        textTexture.render( textX, textY );
    }
}

void Button::setText(const std::string& newText) {
    text = newText;

    if (!text.empty()) {
        if( !textTexture.loadFromRenderedText( text.c_str(), Colors::black ) )
        {
            printf( "Failed to render text texture!\n" );
        }
    } else {
        textTexture.free();
    }
}
