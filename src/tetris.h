#ifndef __TETRIS_H__
#define __TETRIS_H__

#include "sound.h"
#include "timer.h"
#include "input.h"
#include "game_view.h"
#include "game_logic.h"

class Tetris {
public:
    Tetris(int width, int height);
    ~Tetris();

    void start();

private:
    void gameLoop();
    void handlePlayerAction();
    void playMenuSound();
    void gameStep();

    void game();
    void highScore();
    void ingameMenu();
    void mainMenu();
    void settings();
    void texturePacks();

    State state;
    Timer timer;

    Sound* sound;
    Input* input;
    GameView* game_view;
    GameLogic* game_logic;
    HighScore* high_score;

    const unsigned int move_delay_per_level[30] = {
                                        48, 43, 38, 33, 28, 23, 18, 13, 8, 6,
                                        5, 5, 5, 4, 4, 4, 3, 3, 3, 2,
                                        2, 2, 2, 2, 2, 2, 2, 2, 2, 1
    };
};


#endif /*__TETRIS_H__*/