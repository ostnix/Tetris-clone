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

    addLabels();
    menuLoop();

    return state;
}

void Menu::addLabels() {
    render->clearLabels();

    switch (menu) {
    case MenuType::MainMenu:
        render->addLabel(FontType::Big, "Start Game", render->gray, {5, 6});
        render->addLabel(FontType::Big, "Settings", render->gray, {5, 8});
        render->addLabel(FontType::Big, "High Score", render->gray, {5, 10});
        render->addLabel(FontType::Big, "Exit", render->gray, {5, 12});
        break;
    case MenuType::Settings:
        render->addLabel(FontType::Big, "Settings", render->gray, {4, 1});
        render->addLabel(FontType::Normal, "Shadow:", render->gray, {5, 6});
        render->addLabel(FontType::Normal, "Hold Piece:", render->gray, {5, 8});
        render->addLabel(FontType::Normal, "Next Piece:", render->gray, {5, 10});
        render->addLabel(FontType::Normal, "Back", render->gray, {5, 12});
        break;
    case MenuType::InGameMenu:
        render->addLabel(FontType::Big, "Resume", render->gray, {5, 8});
        render->addLabel(FontType::Big, "Exit", render->gray, {5, 10});
        break;
    }
}

void Menu::menuLoop() { 
    while(in_menu && !state.quit_game) {
        SDL_Delay(state.delay);
        keyPressHandler();
        render->renderOnlyLables();

        switch (menu) {
        case MenuType::MainMenu:
            render->showCursor({2, (int)cursor_position * 2 + 6});
            break;
        case MenuType::Settings:
            render->showCursor({2, (int)cursor_position * 2 + 6});
            break;
        case MenuType::InGameMenu:
            render->showCursor({2, (int)cursor_position * 2 + 8});
            break;
        }

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
            addLabels();
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
