#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__

#include <cstdlib>
#include <ctime>
#include "settings.h"
#include "tetromino.h"

class GameLogic {
public:
    GameLogic(State& _state);
    ~GameLogic();

    void start();
    
    bool holdTetromino();
    bool newTetromino();
    bool rotate();
    bool moveDown(bool instantly = false);
    bool moveLeft();
    bool moveRight();

private:
    Tetromino getTetromino();
    
    void putOnGrid(Tetromino& _tetromino);

    bool isValidPosition(Tetromino& _tetromino);
    void cleanFilledLines();
    bool isLineFilled(unsigned int row);
    void updateScoreAndLevel(unsigned int filled_lines);
    void projectShadow();
    void instantDrop();

    State& state;

    int lines_before_next_level = 0;
    const int lines_per_level[30] = {
                                10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
                                100, 100, 100, 100, 100, 100, 110, 120, 130, 140,
                                150, 160, 170, 180, 190, 200, 200, 200, 200, 200
    };

};


#endif /*__GAME_LOGIC_H__*/