#include "tetromino.h"

Tetromino::Tetromino(TetrominoType _type) : type(_type) {
    
    if (type != TetrominoType::None) {
        unsigned int t = (unsigned int)type - 1;
        for (int i = 0; i < num_of_cells; i++) {
            cells[i].col = shapes[t][i][0];
            cells[i].row = shapes[t][i][1];
            cells[i].cell_color = (CellColor)type;
        }
    }
}

Tetromino::Tetromino(const Tetromino& tetromino) {
    num_of_cells = tetromino.num_of_cells;
    type = tetromino.type;

    for (int i = 0; i < num_of_cells; i++) {
        cells[i] = tetromino.cells[i];
    }
}

Tetromino& Tetromino::operator=(const Tetromino& tetromino) {
    num_of_cells = tetromino.num_of_cells;
    type = tetromino.type;

    for (int i = 0; i < num_of_cells; i++) {
        cells[i] = tetromino.cells[i];
    }
    return *this;
}

void Tetromino::rotateCounterClockwise() {
    int col_offset = 0, row_offset = 0;

    for (int i = 1; i < num_of_cells; i++) {
        col_offset = cells[i].row - cells[0].row;
        row_offset = cells[i].col - cells[0].col;
        row_offset *= -1;

        cells[i].col = col_offset + cells[0].col;
        cells[i].row = row_offset + cells[0].row;
    }
}

void Tetromino::rotateClockwise() {
    int col_offset = 0, row_offset = 0;

    for (int i = 1; i < num_of_cells; i++) {
        col_offset = cells[i].row - cells[0].row;
        row_offset = cells[i].col - cells[0].col;
        col_offset *= -1;

        cells[i].col = col_offset + cells[0].col;
        cells[i].row = row_offset + cells[0].row;
    }
}

void Tetromino::moveDown() {
    for (int i = 0; i < num_of_cells; i++) {
        cells[i].row++;
    }
}

void Tetromino::moveUp() {
    for (int i = 0; i < num_of_cells; i++) {
        cells[i].row--;
    }
}

void Tetromino::moveLeft() {
    for (int i = 0; i < num_of_cells; i++) {
        cells[i].col--;
    }
}

void Tetromino::moveRight() {
    for (int i = 0; i < num_of_cells; i++) {
        cells[i].col++;
    }
}

void Tetromino::setColRow(int col, int row) {
    if ((cells[0].col == col) && (cells[0].row == row))
        return;

    int old_col = cells[0].col;
    int old_row = cells[0].row;

    for (int i = 0; i < num_of_cells; i++) {
        cells[i].col -= old_col;
        cells[i].col += col;
        cells[i].row -= old_row;
        cells[i].row += row;
    }
}

void Tetromino::setColor(CellColor color) {
    for (int i = 0; i < num_of_cells; i++) {
        cells[i].cell_color = color;
    }
}

void Tetromino::setTransparency(unsigned char transparency) {
    for (int i = 0; i < num_of_cells; i++) {
        cells[i].transparency = transparency;
    }
}

void Tetromino::setDefaultTransparensy() {
    for (int i = 0; i < num_of_cells; i++) {
        cells[i].transparency = 255;
    }
}

void Tetromino::setDefaultColor() {
    CellColor color = (CellColor)type;

    for (int i = 0; i < num_of_cells; i++) {
        cells[i].cell_color = color;
    }
}

int Tetromino::getCol() {
    return cells[0].col;
}

int Tetromino::getRow() {
    return cells[0].row;
}

CellColor Tetromino::getColor() {
    return cells[0].cell_color;
}