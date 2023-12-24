#ifndef __TETROMINO_H__
#define __TETROMINO_H__

enum class Colors {NONE = 0, BLUE, CYAN, GREEN, ORANGE, RED, VIOLET, YELLOW};
 
static const int shapes[7][4][2] = {
    {{0, 0}, {-1, 0}, {-2,  0}, { 1, 0}},  // shape I
    {{0, 0}, {-1, 0}, { 0,  1}, {-1, 1}},  // shape O
    {{0, 0}, {-1, 0}, { 0, -1}, { 0, 1}},  // shape T
    {{0, 0}, { 1, 0}, { 0,  1}, {-1, 1}},  // shape S
    {{0, 0}, {-1, 0}, { 0,  1}, { 1, 1}},  // shape Z
    {{0, 0}, { 1, 0}, {-1, -1}, {-1, 0}},  // shape J
    {{0, 0}, { 1, 0}, { 1, -1}, {-1, 0}},  // shape L
};

struct Tetromino {     
    void setShape(const int shape[4][2]) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 2; j++) {
                blocks[i][j] = shape[i][j];
            }
        }
    }

    int blocks[4][2];
    int col, row;
    unsigned int color;
};

#endif /* __TETROMINO_H__ */
