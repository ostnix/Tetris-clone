#ifndef __MENU_H__
#define __MENU_H__

#include "settings.h"
#include "high_score.h"
#include "render.h"

class Menu {
public:
    Menu();
    ~Menu();

    State openMenu(Render* _render, State _state, MenuType _menu);

private:
    void menuLoop(); 
    void keyPressHandler();
    void moveCursor(int direction);
    void changeSetting(int direction);
    void action();

    bool in_menu = false;
    unsigned int cursor_position = 0;
    MenuType menu = MenuType::MainMenu;
    State state;
    Render* render;

};

#endif /* __MENU_H__ */
