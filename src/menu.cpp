#include "menu.h"

Menu::Menu() {
}

Menu::~Menu() {
}


State Menu::openMenu(Render* _render, State _state, MenuType _menu) {
    if (_render == NULL)
        return state;
    render = _render;
    in_menu = true;
    cursor_position = 0;
    menu = _menu;
    state = _state;

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
                break;
            
            case SDLK_RIGHT:
            case SDLK_d:
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
            //HighScore high_score;
            //high_score.show();
            break;

        case 3:
            state.quit_game = true;
            break;
        }
        break;
    
    case MenuType::Settings:
        switch (cursor_position) {
        case 0:
            break;
        
        case 1:
            break;

        case 2:
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
