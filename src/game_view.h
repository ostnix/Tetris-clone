#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "settings.h"
#include "tetromino.h"
#include "render.h"
#include "high_score.h"

class GameView {
public:
    GameView(int width, int height);
    ~GameView();

    void draw(State& state);
    void updateGrid(State& state);
    void updateScreen(State& state);


private:
    void redrawBackground();
    void showPause();
    void createTexts();
    void putTetromino(Tetromino& tetromino, LayerId id);
    void drawGame(State& state);
    void drawIngameMenu(State& state);
    void drawMainMenu(State& state);
    void drawSettings(State& state);
    void drawHighScore(State& state);
    
    //Screen getCursorPosition(Screen init_pos, int step_size, unsigned int items_in_menu, unsigned int position);


    enum Texts {Next = 0, Holded = 1, Pause = 2, Score = 3, Level = 4, ScoreVal = 5, 
                LevelVal = 6, NewGame = 7, Settings = 8, HighScore = 9, Exit = 10, 
                Resume = 11, MainMenu = 12, On = 13, Off = 14, Shadow = 15, Back = 16};

    LayerId game_view = -1;
    LayerId game_grid = -1;
    LayerId ingame_menu = -1;
    LayerId main_menu = -1;

    TextId records[HIGH_SCORE_PLAYERS_NUMBER] = {-1};
    TextId texts[MAX_TEXTS] = {-1};

    Render* render;
};


#endif /* __GAME_VIEW_H__ */
