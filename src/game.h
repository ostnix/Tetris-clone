#ifndef __GAME_H__
#define __GAME_H__

#include <cstdlib>
#include <ctime>
#include "settings.h"
#include "tetromino.h"
#include "render.h"
#include "menu.h"

class Game {
public:
    Game(int width, int height);
    Game(int width, int height, unsigned int start_level);
    ~Game();

    void start();
    
private:
    void createLabels();
    void gameLoop();
    void keyPressHandler();

    bool spawnTetromino();
    void holdTetromino();
    void generateNextTetromino();
    void placeTetromino();
    void createShadow();
    void projectShadow();
    void rotateCounterClockwise();
    void moveLeft();
    void moveRight();
    void moveDown();
    void dropInstantly();
    void cleanFilledLines();
    void updateScoreAndLevel(int filled_lines);
    bool isLineFilled(int row);
    bool isValidPosition(const Tetromino& _tetromino);
    bool isValidPosition(int col, int row);
    bool isValidRotation(int blocks[4][2]); 
 
    Tetromino tetromino;
    Tetromino next_tetromino;
    Tetromino holded_tetromino;
    Tetromino shadow_tetromino;
    int score_label_index = -1;
    int level_label_index = -1;

    unsigned int grid[10][20] = {0}; 
    int lines_per_level[30] =   {
                                10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
                                100, 100, 100, 100, 100, 100, 110, 120, 130, 140,
                                150, 160, 170, 180, 190, 200, 200, 200, 200, 200
                                };
    unsigned int move_delay_per_level[30] = {
                                            48, 43, 38, 33, 28, 23, 18, 13, 8, 6,
                                            5, 5, 5, 4, 4, 4, 3, 3, 3, 2,
                                            2, 2, 2, 2, 2, 2, 2, 2, 2, 1
                                            };
    State state;
    Menu menu;
    Render* render;
};



#endif /* __GAME_H__ */
