#ifndef __TETRIS_H__
#define __TETRIS_H__

#include "timer.h"
#include "input.h"
#include "game.h"
#include "game_logic.h"

class Tetris {
public:
    Tetris(int width, int height, unsigned int start_level = 0);
    ~Tetris();

    void start();

private:
    void gameLoop();
    void handlePlayerAction();
    void workoutGameContext();

    State state;
    Input input;
    Timer timer;

    GameView* game;
    GameLogic* game_logic;

    unsigned int move_delay_per_level[30] = {
                                        48, 43, 38, 33, 28, 23, 18, 13, 8, 6,
                                        5, 5, 5, 4, 4, 4, 3, 3, 3, 2,
                                        2, 2, 2, 2, 2, 2, 2, 2, 2, 1
    };
};


#endif /*__TETRIS_H__*/