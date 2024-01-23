#ifndef __TETROMINO_H__
#define __TETROMINO_H__

static const unsigned int MAX_BLOCKS_IN_TETROMINO = 4;

enum class CellColor {
    None = 0,
    Cyan = 1,
    Yellow = 2,
    Violet = 3,
    Green = 4,
    Red = 5,
    Blue = 6,
    Orange = 7,
    Blank = 8,
    Grey = 9
};

struct Cell {
    CellColor cell_color = CellColor::None;
    unsigned char transparency = 255;
    bool movable = false;
    int col = 0, row = 0;
};

enum class TetrominoType {None = 0,  
                    I = 1, 
                    O = 2, 
                    T = 3, 
                    S = 4, 
                    Z = 5, 
                    J = 6, 
                    L = 7};
 
static const int shapes[7][4][2] = {       // shapes with 4 blocks
    {{0, 0}, {-1, 0}, {-2,  0}, { 1, 0}},  // shape I
    {{0, 0}, {-1, 0}, { 0,  1}, {-1, 1}},  // shape O
    {{0, 0}, {-1, 0}, { 0, -1}, { 0, 1}},  // shape T
    {{0, 0}, { 1, 0}, { 0,  1}, {-1, 1}},  // shape S
    {{0, 0}, {-1, 0}, { 0,  1}, { 1, 1}},  // shape Z
    {{0, 0}, { 1, 0}, {-1, -1}, {-1, 0}},  // shape J
    {{0, 0}, { 1, 0}, { 1, -1}, {-1, 0}},  // shape L
};

class Tetromino {     
public:
    Tetromino(TetrominoType _type = TetrominoType::None);
    Tetromino(const Tetromino& tetromino);

    Tetromino& operator=(const Tetromino& tetromino);

    void rotateCounterClockwise();
    void rotateClockwise();
    void moveDown();
    void moveUp();
    void moveLeft();
    void moveRight();
    void setColRow(int col, int row);
    void setColor(CellColor color);
    void setTransparency(unsigned char transparency = 255);
    void setDefaultTransparensy();
    void setDefaultColor();
    
    int getCol();
    int getRow();
    
    CellColor getColor();

    TetrominoType type = TetrominoType::None;
    Cell cells[MAX_BLOCKS_IN_TETROMINO];
    unsigned int num_of_cells = 4;

private:
};

#endif /* __TETROMINO_H__ */
