#include "game.h"

Game::Game(int width, int height) : Game(width, height, 0) {

}

Game::Game(int width, int height, unsigned int start_level) {
    render = new Render(width, height);
    if (start_level <= 29)
        state.level = start_level;
    else
        state.level = 29;

    high_score = new HighScore(render);
    menu = new Menu(render, high_score);
}

Game::~Game() {
    delete menu;
    delete high_score;
    delete render;
}

void Game::start() {
    state.delay = 1000/FRAME_RATE; 

    state = menu->openMenu(state, MenuType::MainMenu);
    createLabels();

    srand(time(NULL));

    generateNextTetromino();
    holded_tetromino.color = 0;
    shadow_tetromino.color = 0;
    gameLoop();
}

void Game::createLabels() {
    render->clearLabels();
    score_label_index = -1;
    level_label_index = -1;

    render->addLabel(FontType::Normal, "Next", render->gray, {12, 0});
    render->addLabel(FontType::Normal, "Score:", render->gray, {12, 8});
    score_label_index = render->addLabel(FontType::Normal, state.score, render->gray, {16,8});
    render->addLabel(FontType::Normal, "Level:", render->gray, {12, 10});
    level_label_index = render->addLabel(FontType::Normal, state.level, render->gray, {16,10});
    render->addLabel(FontType::Normal, "Holded piece", render->gray, {12, 15});

}

void Game::gameLoop() {
     unsigned int timer = move_delay_per_level[state.level];

     spawnTetromino();
     
     while(!state.quit_game && !state.game_ended) {      
        keyPressHandler(); 
        
        if (state.paused)
            continue;

        if (timer == 0 || state.soft_drop) {
            moveDown();
            state.rerender = true;
            timer = move_delay_per_level[state.level];
        }
        
        if (state.should_be_placed) {
            placeTetromino();
            cleanFilledLines();
            state.soft_drop = false;

            if (!spawnTetromino()) {
                render->popupLabel(FontType::Big, "Game Ended", render->white, {5,10});
                high_score->checkForNewRecord(state);
                high_score->show(state);
                printf("GAME ENDED!!!\n"); 
                state.game_ended = true;
            }
        }
        
        if (state.rerender) { 
            render->renderGame(grid, tetromino, next_tetromino, holded_tetromino, shadow_tetromino, state.show_next_piece_enabled);
            state.rerender = false;
        }

        SDL_Delay(state.delay);
        --timer;
     }
}

void Game::keyPressHandler() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            state.quit_game = true;
            break;
        case SDL_KEYDOWN:
            state.paused = false;
            state.rerender = true;
            switch(event.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
                moveLeft();
                break;
            
            case SDLK_RIGHT:
            case SDLK_d:
                moveRight();
                break;
            
            case SDLK_UP:
            case SDLK_w:
                rotateCounterClockwise();
                break;
            
            case SDLK_DOWN:
            case SDLK_s:
                state.soft_drop = true;
                break;

            case SDLK_p:
                render->popupLabel(FontType::Big, "Pause", render->white, {5,10});
                state.paused = true;
                break;
            
            case SDLK_h:
                holdTetromino();
                break;

            case SDLK_SPACE:
                dropInstantly();
                break;

            case SDLK_ESCAPE:
                state = menu->openMenu(state, MenuType::InGameMenu);
                createLabels();
                break;
            }            
        }
   }
}

bool Game::spawnTetromino() {
    if (holded_tetromino.color == BLOCKED_COLOR)
        holded_tetromino.color = state.backup_color;

    tetromino.setShape(next_tetromino.blocks);
    tetromino.col = 5;
    tetromino.row = 0;
    tetromino.color = next_tetromino.color;

    generateNextTetromino();

    createShadow();

    return isValidPosition(5, 0);
}

void Game::holdTetromino() {
    if (!state.hold_piece_enabled)
        return;

    if (holded_tetromino.color == 0) {                          // No tetromino holded
        holded_tetromino.col = 15;
        holded_tetromino.row = 18;
        holded_tetromino.color = BLOCKED_COLOR;
        state.backup_color = tetromino.color;
        holded_tetromino.setShape(tetromino.blocks);
        spawnTetromino(); 
    }
    else if (holded_tetromino.color != BLOCKED_COLOR) {         // Tetromino is not blocked
        Tetromino _tetromino;
        _tetromino.col = tetromino.col;
        _tetromino.row = tetromino.row;
        
        _tetromino.color = holded_tetromino.color;
        _tetromino.setShape(holded_tetromino.blocks);

        if (isValidPosition(_tetromino)) {                      // Swaping ingame and holded tetrominoes
            state.backup_color = tetromino.color;
            holded_tetromino.color = BLOCKED_COLOR;
            holded_tetromino.setShape(tetromino.blocks);

            tetromino.color = _tetromino.color;
            tetromino.setShape(_tetromino.blocks);

            createShadow();
        }
    }    
}

void Game::generateNextTetromino() {
    int random = rand() % 7;
    next_tetromino.setShape(shapes[random]);
    next_tetromino.col = 15;
    next_tetromino.row = 3;
    next_tetromino.color = random + 1;
}

void Game::placeTetromino() {
    for (int i = 0; i < 4; i++) {
        grid[tetromino.col + tetromino.blocks[i][0]]
            [tetromino.row + tetromino.blocks[i][1]] 
            = tetromino.color; 
    }
    state.should_be_placed = false;
}

void Game::createShadow() {
    if (!state.shadow_enabled)
        return;

    shadow_tetromino.col = tetromino.col;
    shadow_tetromino.color = tetromino.color;
    shadow_tetromino.setShape(tetromino.blocks);
    projectShadow();
}

void Game::projectShadow() {
    if (!state.shadow_enabled)
        return;

    shadow_tetromino.col = tetromino.col;

    for (int i = 0; i < 20; i++) {
        shadow_tetromino.row = i;
        if (!isValidPosition(shadow_tetromino)) {
            --shadow_tetromino.row;
            break;
        }
    }
}

void Game::rotateCounterClockwise() {
    int blocks[4][2];
    blocks[0][0] = 0;
    blocks[0][1] = 0;

    for (int i = 1; i < 4; i++) {
        blocks[i][0] = tetromino.blocks[i][1];
        blocks[i][1] = tetromino.blocks[i][0];

        if (tetromino.blocks[i][0] != 0)
            blocks[i][1] *= -1;  
    }
    
    if (isValidRotation(blocks)) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 2; j++) {
                tetromino.blocks[i][j] = blocks[i][j];

                if (state.shadow_enabled) {
                    shadow_tetromino.blocks[i][j] = blocks[i][j];
                }
            }       
        }

        projectShadow();
    }
}

void Game::moveLeft() {
    if (isValidPosition(tetromino.col - 1, tetromino.row)) {
        tetromino.col -= 1; 
        projectShadow();
    }
}

void Game::moveRight() {
    if (isValidPosition(tetromino.col + 1, tetromino.row)) {
        tetromino.col += 1;
        projectShadow();
    }
}

void Game::moveDown() {
    if (isValidPosition(tetromino.col, tetromino.row + 1)) {
        tetromino.row += 1;
        return;
    }

    state.should_be_placed = true;
}

void Game::dropInstantly() {
    while(!state.should_be_placed) {
        moveDown();
    }
}

void Game::cleanFilledLines() {
    int filled_lines = 0;
    
    for (int row = 19; row >= 0; row--) {
        if (isLineFilled(row)) {
            filled_lines++;
        }
        else if (filled_lines > 0) {
            for (int col = 0; col < 10; col++) {
                assert(row + filled_lines < 20);
                grid[col][row + filled_lines] = grid[col][row];
            }
        }
    }

    for (int row = 0; row < filled_lines; row++) {
        for (int col = 0; col < 10; col++) {
            assert(row + filled_lines < 20);
            grid[col][row] = 0;        
        }
    }
    
    if (filled_lines > 0)
        updateScoreAndLevel(filled_lines);
}


void Game::updateScoreAndLevel(int filled_lines) {
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
    if (state.level < 29) {
        lines_per_level[state.level] -= filled_lines;
        if (lines_per_level[state.level] <= 0)
            ++state.level;
    }

    render->updateLabel(state.score, score_label_index);
    render->updateLabel(state.level, level_label_index);
}

bool Game::isLineFilled(int row) {
    assert(row >= 0 && row < 20);
    for (int col = 0; col < 10; col++) {
        if (grid[col][row] == 0)
            return false;
    }
    return true;
}

bool Game::isValidPosition(const Tetromino& _tetromino) {
    for (int i = 0; i < 4; i++) {
        if (_tetromino.col + _tetromino.blocks[i][0] < 0   ||
            _tetromino.col + _tetromino.blocks[i][0] >= 10 ||
            _tetromino.row + _tetromino.blocks[i][1] >= 20)
            return false;
       
        if ((_tetromino.row + _tetromino.blocks[i][1]) >= 0 ) {
            if (grid[_tetromino.col + _tetromino.blocks[i][0]][_tetromino.row + _tetromino.blocks[i][1]] > 0)
                return false;
        }
    }

    return true;
}

bool Game::isValidPosition(int col, int row) {
    for (int i = 0; i < 4; i++) {
        if (col + tetromino.blocks[i][0] < 0   ||
            col + tetromino.blocks[i][0] >= 10 ||
            //row + tetromino.blocks[i][1] < 0   ||
            row + tetromino.blocks[i][1] >= 20)
            return false;
       
        if ((row + tetromino.blocks[i][1]) >= 0 ) {
            if (grid[col + tetromino.blocks[i][0]][row + tetromino.blocks[i][1]] > 0)
                return false;
        }
    }

    return true;
}

bool Game::isValidRotation(int blocks[4][2]) {
    for (int i = 0; i < 4; i++) {
        if (tetromino.col + blocks[i][0] < 0   ||
            tetromino.col + blocks[i][0] >= 10 ||
            //tetromino.row + blocks[i][1] < 0   ||
            tetromino.row + blocks[i][1] >= 20)
            return false;
       
        if ((tetromino.row + blocks[i][1]) >= 0) {
            if (grid[tetromino.col + blocks[i][0]][tetromino.row + blocks[i][1]] > 0)
                return false;
        }
    }

    return true;
}

