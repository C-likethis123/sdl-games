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

Button::Button(const std::string& text, SDL_FRect rect, std::function<void()> func) : text(text), func(std::move(func)), rect(rect) {
    initialise();
}

void Button::initialise() {
    // Only render text if it's not empty
    if (!text.empty()) {
        SDL_Color textColor{ 0, 0, 0 };
        if( !textTexture.loadFromRenderedText( text.c_str(), textColor ) )
        {
            printf( "Failed to render text texture!\n" );
        }
        
        // When text is present, always auto-size to fit the text
        // This ensures the clickable area matches the visible text
        rect.w = textTexture.getWidth();
        rect.h = textTexture.getHeight();
    }
    // If text is empty, keep the explicit dimensions provided by constructor
    // This supports buttons that will have text added later (like tic-tac-toe grid)
}

void Button::render() {
    // Only render if there's text to show
    if (!text.empty()) {
        textTexture.render( rect.x,  rect.y );
    }
}

void Button::setText(const std::string& newText) {
    text = newText;
    
    // Re-render the text texture
    if (!text.empty()) {
        SDL_Color textColor{ 0, 0, 0 };
        if( !textTexture.loadFromRenderedText( text.c_str(), textColor ) )
        {
            printf( "Failed to render text texture!\n" );
        }
    } else {
        // Clear the texture if text is empty
        textTexture.free();
    }
}
