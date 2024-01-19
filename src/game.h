#ifndef __GAME_H__
#define __GAME_H__

#include "settings.h"
#include "tetromino.h"
#include "render.h"

class Game {
public:
    Game(int width, int height);
    ~Game();

    void draw(State& state);

    void updateGrid(State& state);
    void updateScreen(State& state);

private:
    void updateBackground();
    void showPause();
    void createTexts();
    void putTetromino(Tetromino& tetromino, LayerId id);

    enum Texts {Next = 0, Holded = 1, Pause = 2, Score = 3, Level = 4, ScoreVal = 5, 
                LevelVal = 6, NewGame = 7, Settings = 8, HighScore = 9, Exit = 10, 
                Resume = 11, MainMenu = 12, On = 13, Off = 14, Shadow = 15};

    LayerId game_view = -1;
    LayerId game_grid = -1;
    LayerId ingame_menu = -1;
    LayerId main_menu = -1;

    TextId texts[MAX_TEXTS] = {-1};

    Render* render;
};


#endif /* __GAME_H__ */
