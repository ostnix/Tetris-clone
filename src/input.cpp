#include "input.h"

PlayerAction Input::getEvent(char* text_input) {
    SDL_Event event;

    if (!SDL_PollEvent(&event)) {
        return PlayerAction::None;
    }

    switch (event.type) {
    case SDL_QUIT:
        return PlayerAction::Exit;
        break;
    
    case SDL_TEXTINPUT:
        strcpy(text_input, event.text.text);
        return PlayerAction::OtherKey;
        break;

    case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_LEFT:
        case SDLK_a:
            return PlayerAction::Left;
            break;

        case SDLK_RIGHT:
        case SDLK_d:
            return PlayerAction::Right;
            break;

        case SDLK_UP:
        case SDLK_w:
            return PlayerAction::Up;
            break;

        case SDLK_DOWN:
        case SDLK_s:
            return PlayerAction::Down;
            break;

        case SDLK_p:
            return PlayerAction::Pause;
            break;

        case SDLK_h:
            return PlayerAction::HoldPiece;
            break;

        case SDLK_SPACE:
            return PlayerAction::FastDrop;
            break;

        case SDLK_ESCAPE:
            return PlayerAction::ESC;
            break;

        case SDLK_RETURN:
            return PlayerAction::Enter;
            break;

        case SDLK_BACKSPACE:
            return PlayerAction::BackSpace;
            break;

        case SDLK_RALT:
        case SDLK_LALT:
        case SDLK_PRINTSCREEN:
            return PlayerAction::SystemKey;
            break;

        default:
            return PlayerAction::OtherKey;
            break;
        }
        break;
    }
    return PlayerAction::None;
}

void Input::keyboardTextInput(bool active) {
    if (active) {
        SDL_StartTextInput();
    }
    else {
        SDL_StopTextInput();
    }
}
