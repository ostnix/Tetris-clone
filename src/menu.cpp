#include "menu.h"

Menu::Menu() {
}

Menu::~Menu() {
}


State Menu::openMenu(Render* _render, State _state, MenuType _menu) {
    render = _render;
    state = _state;
    if (render == NULL)
        return state;

    in_menu = true;
    cursor_position = 0;
    menu = _menu;

    menuLoop();

    return state;
}

void Menu::menuLoop() { 
    while(in_menu && !state.quit_game) {
        SDL_Delay(state.delay);
        keyPressHandler();
        render->showMenu(menu, cursor_position);
    }
}

void Menu::keyPressHandler() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            state.quit_game = true;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
                changeSetting(-1);
                break;
            
            case SDLK_RIGHT:
            case SDLK_d:
                changeSetting(1);
                break;
            
            case SDLK_UP:
            case SDLK_w:
                moveCursor(-1);
                break;
            
            case SDLK_DOWN:
            case SDLK_s:
                moveCursor(1);
                break;

            case SDLK_RETURN:
                action();
                break;

            case SDLK_ESCAPE:
                break;
            }            
        }
   }

}

void Menu::moveCursor(int direction) {
    switch (menu) {
    case MenuType::MainMenu: 
        cursor_position = (cursor_position + direction) % 4;
        break;   

    case MenuType::Settings: 
        cursor_position = (cursor_position + direction) % 4;
        break;   
    
    case MenuType::InGameMenu:
        cursor_position = (cursor_position + direction) % 2;
        break;   
    }
}

void Menu::changeSetting(int direction) {
    switch (menu) {
    case MenuType::Settings:
        switch (cursor_position) {
        case 0:                     // shadow on/off
            state.shadow_enabled = !state.shadow_enabled;
            break;

        case 1:                     // hold piece on/off
            state.hold_piece_enabled = !state.hold_piece_enabled;
            break;

        case 2:                     // show next piece on/off
            state.show_next_piece_enabled = !state.show_next_piece_enabled;
            break;
        }
        break;
    }
}

void Menu::action() {
    switch (menu) {
    case MenuType::MainMenu:
        switch (cursor_position) {
        case 0:
            in_menu = false;
            break;
        
        case 1: {
            Menu settings;
            state = settings.openMenu(render, state, MenuType::Settings);
            break;
        }
        case 2:
            state = HighScore::show(render, state);
            break;

        case 3:
            state.quit_game = true;
            break;
        }
        break;
    
    case MenuType::Settings:
        switch (cursor_position) {
        case 0:                     // shadow on/off
            changeSetting(0);
            break;
        
        case 1:                     // hold piece on/off
            changeSetting(0);
            break;

        case 2:                     // show next piece on/off
            changeSetting(0);
            break;

        case 3:
            in_menu = false;
            break;
        }

        break;   
    
    case MenuType::InGameMenu:
        switch (cursor_position) {
        case 0:
            in_menu = false;
            break;
        
        case 1:
            state.quit_game = true;
            break;
        }
        
        break;
    }
}
