#include "game_logic.h"

GameLogic::GameLogic(State& _state) : state(_state) {
    srand(time(NULL));
}

GameLogic::~GameLogic() {

}

void GameLogic::start() {
    if (state.level < 30)
        lines_before_next_level = lines_per_level[state.level];
    else 
        lines_before_next_level = lines_per_level[29];
    
    for (int col = 0; col < GAME_FIELD_WIDTH; col++) {
        for (int row = 0; row < GAME_FIELD_HEIGHT; row++) {
            state.grid[col][row].col = col;
            state.grid[col][row].row = row;
            state.grid[col][row].cell_color = CellColor::None;
            state.grid[col][row].movable = false;
            state.grid[col][row].transparency = 255;
        }
    }

    state.tetrominoes[ACTIVE] = getTetromino();
    state.tetrominoes[ACTIVE].setColRow(5, 0);
    state.tetrominoes[NEXT] = getTetromino();
    state.tetrominoes[HOLDED].setColor(CellColor::None);
    projectShadow();
}

bool GameLogic::holdTetromino() {
    if (state.hold_enabled) {
        if (state.tetrominoes[HOLDED].type == state.tetrominoes[ACTIVE].type) {
            return false;
        }

        if (state.tetrominoes[HOLDED].getColor() == CellColor::None) {
            state.tetrominoes[HOLDED] = state.tetrominoes[ACTIVE];
            state.tetrominoes[HOLDED].setColor(CellColor::Blank);
            state.tetrominoes[ACTIVE] = state.tetrominoes[NEXT];
            state.tetrominoes[ACTIVE].setColRow(5, 0);
            state.tetrominoes[NEXT] = getTetromino();
            projectShadow();
        }
        else if (state.tetrominoes[HOLDED].getColor() != CellColor::Blank) {
            Tetromino tetromino = state.tetrominoes[ACTIVE];
            state.tetrominoes[ACTIVE] = state.tetrominoes[HOLDED];
            state.tetrominoes[ACTIVE].setColRow(tetromino.getCol(), tetromino.getRow());
            state.tetrominoes[HOLDED] = tetromino;
            state.tetrominoes[HOLDED].setColor(CellColor::Blank);
            projectShadow();
        }
        return true;
    }
    return false;
}

bool GameLogic::newTetromino() {
    putOnGrid(state.tetrominoes[ACTIVE]);
    cleanFilledLines();
    state.tetrominoes[ACTIVE] = state.tetrominoes[NEXT];
    state.tetrominoes[ACTIVE].setColRow(5, 0);
    state.tetrominoes[NEXT] = getTetromino();

    if (state.tetrominoes[HOLDED].getColor() == CellColor::Blank) {
        state.tetrominoes[HOLDED].setDefaultColor();
    }
    
    projectShadow();
    
    return isValidPosition(state.tetrominoes[ACTIVE]);
}

bool GameLogic::rotate() {
    state.tetrominoes[ACTIVE].rotateCounterClockwise();
    if (!isValidPosition(state.tetrominoes[ACTIVE])) {
        state.tetrominoes[ACTIVE].rotateClockwise();
        return false;
    }

    projectShadow();
    return true;
}

bool GameLogic::moveDown(bool instantly) {
    if (instantly) {
        instantDrop();
        return false;
    }

    state.tetrominoes[ACTIVE].moveDown();
    if (!isValidPosition(state.tetrominoes[ACTIVE])) {
        state.tetrominoes[ACTIVE].moveUp();
        return false;
    }

    return true;
}

bool GameLogic::moveLeft() {
    state.tetrominoes[ACTIVE].moveLeft();
    if (!isValidPosition(state.tetrominoes[ACTIVE])) {
        state.tetrominoes[ACTIVE].moveRight();
        return false;
    }
    
    projectShadow();
    return true;
}

bool GameLogic::moveRight() {
    state.tetrominoes[ACTIVE].moveRight();
    if (!isValidPosition(state.tetrominoes[ACTIVE])) {
        state.tetrominoes[ACTIVE].moveLeft();
        return false;
    }

    projectShadow();
    return true;
}

// private

Tetromino GameLogic::getTetromino() {
    return Tetromino((TetrominoType)((rand() % 7) + 1));
}

void GameLogic::putOnGrid(Tetromino& _tetromino) {
    for (int i = 0; i < _tetromino.num_of_cells; i++) {
        if (_tetromino.cells[i].col >= 0 && _tetromino.cells[i].col < GAME_FIELD_WIDTH && _tetromino.cells[i].row >= 0 && _tetromino.cells[i].row < GAME_FIELD_HEIGHT)
            state.grid[_tetromino.cells[i].col][_tetromino.cells[i].row].cell_color = _tetromino.cells[i].cell_color;
    }
}

bool GameLogic::isValidPosition(Tetromino& _tetromino) {
    for (int i = 0; i < _tetromino.num_of_cells; i++) {
        if (_tetromino.cells[i].row >= GAME_FIELD_HEIGHT ||
            _tetromino.cells[i].col < 0 ||
            _tetromino.cells[i].col >= GAME_FIELD_WIDTH)
            return false;

        if (_tetromino.cells[i].col >= 0 && _tetromino.cells[i].col < GAME_FIELD_WIDTH && _tetromino.cells[i].row >= 0 && _tetromino.cells[i].row < GAME_FIELD_HEIGHT) {
            if (state.grid[_tetromino.cells[i].col][_tetromino.cells[i].row].cell_color != CellColor::None)
                return false;
        }
    }

    return true;
}

void GameLogic::cleanFilledLines() {
    unsigned int filled_lines = 0;

    for (int row = GAME_FIELD_HEIGHT - 1; row >= 0; row--) {
        if (isLineFilled(row)) {
            filled_lines++;
        }
        else if (filled_lines > 0) {
            for (int col = 0; col < GAME_FIELD_WIDTH; col++) {
                state.grid[col][row + filled_lines].cell_color = state.grid[col][row].cell_color;
            }
        }
    }

    for (int row = 0; row < filled_lines; row++) {
        for (int col = 0; col < GAME_FIELD_WIDTH; col++) {
            state.grid[col][row].cell_color = CellColor::None;
        }
    }

    if (filled_lines > 0)
        updateScoreAndLevel(filled_lines);
}

bool GameLogic::isLineFilled(unsigned int row) {
    for (int col = 0; col < GAME_FIELD_WIDTH; col++) {
        if (state.grid[col][row].cell_color == CellColor::None)
            return false;
    }
    return true;
}

void GameLogic::updateScoreAndLevel(unsigned int filled_lines) {
    int score_multiplyer = 0;

    switch (filled_lines) {
    case 1:
        score_multiplyer = 40;
        break;
    case 2:
        score_multiplyer = 100;
        break;
    case 3:
        score_multiplyer = 300;
        break;
    defaut:
        score_multiplyer = 1200;
        break;
    }
    
    state.score += score_multiplyer * (state.level + 1);
    state.update_score = true;
    if (state.level < 29) {
        lines_before_next_level -= filled_lines;
        if (lines_before_next_level <= 0) {
            state.level++;
            lines_before_next_level = lines_per_level[state.level];
            state.update_level = true;
        }
    }
}

void GameLogic::projectShadow() {
    state.tetrominoes[SHADOW] = state.tetrominoes[ACTIVE];
    state.tetrominoes[SHADOW].setTransparency(30);
    while (isValidPosition(state.tetrominoes[SHADOW])) {
        state.tetrominoes[SHADOW].moveDown();
    }
    state.tetrominoes[SHADOW].moveUp();
}

void GameLogic::instantDrop() {
    state.tetrominoes[SHADOW].setColor(CellColor::None);
    state.tetrominoes[ACTIVE].setColRow(state.tetrominoes[SHADOW].getCol(), 
                                        state.tetrominoes[SHADOW].getRow());

}