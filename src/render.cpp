#include "render.h"

Render::Render(int width, int height) : window_width(width), window_height(height) {
    TTF_Init();

    assert(SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window, &renderer) == 0);

    assert(loadTextures());
    assert(loadFonts());
    prerenderText();

    border_width = ((float)window_width/(12*BORDER_SIZE*2)) * BORDER_SIZE;
    border_height = ((float)window_height/(22*BORDER_SIZE)) * BORDER_SIZE ;
    block_width = ((float)window_width/(12*BORDER_SIZE*2)) * BLOCK_SIZE;
    block_height = ((float)window_height/(22*BORDER_SIZE)) * BLOCK_SIZE;

}

Render::~Render() {
    TTF_CloseFont(font);
    TTF_CloseFont(font_big);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Render::renderGame(const unsigned int grid[10][20], 
                        const Tetromino& tetromino, 
                        const Tetromino& next_tetromino,
                        const Tetromino& holded_tetromino,
                        const Tetromino& shadow_tetromino) {

    SDL_RenderClear(renderer);
    renderBackground();
    renderBorders();
    renderText();
    renderGrid(grid);
    renderTetromino(shadow_tetromino, true, true);
    renderTetromino(tetromino, true, false); 
    renderTetromino(next_tetromino, false, false);
    renderTetromino(holded_tetromino, false, false);
    SDL_RenderPresent(renderer);
}

void Render::showPause() {
    SDL_Point size = labels[Labels::Pause]->getSufaceSize();

    labels[Labels::Pause]->render(renderer,{window_width/2 - size.x/2, 
                                            window_height/2 - size.y/2});

    SDL_RenderPresent(renderer);
}

void Render::prerenderScoreAndLevel(int score, int level) {
    char c_score[MAX_TEXT_CHARS + sizeof(char)];
    char c_level[MAX_TEXT_CHARS + sizeof(char)];
    
    sprintf(c_score, "%d", score);
    sprintf(c_level, "%d", level);

    if (dynamic_labels[DynamicLabels::ScoreValue] != NULL)
        dynamic_labels[DynamicLabels::ScoreValue]->updateText(renderer, font, c_score, grey);
    else 
        dynamic_labels[DynamicLabels::ScoreValue] = new Label(renderer, font, c_score, grey);

    if (dynamic_labels[DynamicLabels::LevelValue] != NULL) 
        dynamic_labels[DynamicLabels::LevelValue]->updateText(renderer, font, c_level, grey);

    else 
        dynamic_labels[DynamicLabels::LevelValue] = new Label(renderer, font, c_level, grey);

}

void Render::showHighScore(NamesList names_list) {
    for (int i = 0; i < HIGH_SCORE_PLAYERS_NUMBER; i++) {
        if (players_names[i] == NULL) {
            players_names[i] = new Label(renderer, font, names_list.name[i], blue);
        }
    }
    
    SDL_RenderClear(renderer);
    renderBackground();
    renderHighScore();
    SDL_RenderPresent(renderer);
}

void Render::showPlayerName() {

}

void Render::showMenu(MenuType menu, unsigned int cursor_position) {
    switch (menu) {
    case MenuType::MainMenu:
        SDL_RenderClear(renderer);
        renderBackground();
        renderMainMenu();
        renderCursor(cursor_position, menu);
        SDL_RenderPresent(renderer);
        break;
    
    case MenuType::Settings:
        SDL_RenderClear(renderer);
        renderBackground();
        renderSettings();
        renderCursor(cursor_position, menu);
        SDL_RenderPresent(renderer);
        break;
    
    case MenuType::InGameMenu:
        SDL_RenderClear(renderer);
        renderBackground();
        renderInGameMenu();
        renderCursor(cursor_position, menu);
        SDL_RenderPresent(renderer);
        break;
    }
}

void Render::showGameEnded() {
    int x, y;
    x = 5 * block_width + border_width; y = 10 * block_height + border_height;
    labels[Labels::GameEnded]->render(renderer, {x, y});  
    SDL_RenderPresent(renderer);
}

bool Render::loadTextures() {
    
    block_colors[1] = IMG_LoadTexture(renderer, "./textures/cyan.png");     
    block_colors[2] = IMG_LoadTexture(renderer, "./textures/yellow.png"); 
    block_colors[3] = IMG_LoadTexture(renderer, "./textures/violet.png");
    block_colors[4] = IMG_LoadTexture(renderer, "./textures/green.png");     
    block_colors[5] = IMG_LoadTexture(renderer, "./textures/red.png");
    block_colors[6] = IMG_LoadTexture(renderer, "./textures/blue.png");
    block_colors[7] = IMG_LoadTexture(renderer, "./textures/orange.png");
    block_colors[8] = IMG_LoadTexture(renderer, "./textures/blocked.png");

    background_texture = IMG_LoadTexture(renderer, "./textures/background.png");
    borders_texture = IMG_LoadTexture(renderer, "./textures/borders.png");
    cursor = IMG_LoadTexture(renderer, "./textures/cursor.png");

    for (int i = 1; i < 9; i++) {
        SDL_SetTextureBlendMode(block_colors[i], SDL_BLENDMODE_BLEND);
    }

    return (block_colors[1] && 
            block_colors[2] && 
            block_colors[3] && 
            block_colors[4] && 
            block_colors[5] && 
            block_colors[6] && 
            block_colors[7] &&
            block_colors[8] &&
            background_texture &&
            borders_texture &&
            cursor);
}

bool Render::loadFonts() {
    font = TTF_OpenFont("./fonts/VT323-Regular.ttf", 40);
    font_big = TTF_OpenFont("./fonts/VT323-Regular.ttf", 80);

    if (font == NULL || font_big == NULL) 
        return false;
    
    return true;
}

void Render::renderHighScore() {
    int x, y;
    x = 5 * block_width + border_width;
    y = 2 * block_height + border_height;
    labels[Labels::HighScore]->render(renderer, {x, y});

    for (int i = 0; i < HIGH_SCORE_PLAYERS_NUMBER; i++) {
        x = 5 * block_width + border_width; y = (6 + i*2) * block_height + border_height;
        players_names[i]->render(renderer, {x, y});
    }
}

void Render::renderMainMenu() {
    int x, y;
    x = 5 * block_width + border_width; y = 6 * block_height + border_height;
    labels[Labels::StartGame]->render(renderer, {x, y});

    x = 5 * block_width + border_width; y = 8 * block_height + border_height;
    labels[Labels::Settings]->render(renderer, {x, y});

    x = 5 * block_width + border_width; y = 10 * block_height + border_height;
    labels[Labels::HighScore]->render(renderer, {x, y});

    x = 5 * block_width + border_width; y = 12 * block_height + border_height;    
    labels[Labels::Exit]->render(renderer, {x, y});
}

void Render::renderSettings() {
    int x, y;
    x = 4 * block_width + border_width; y = 1 * block_height + border_height;
    labels[Labels::Settings]->render(renderer, {x, y});

    x = 5 * block_width + border_width; y = 6 * block_height + border_height;
    labels[Labels::ShadowEnabled]->render(renderer, {x, y});

    x = 5 * block_width + border_width; y = 8 * block_height + border_height;    
    labels[Labels::HoldPieceEnabled]->render(renderer, {x, y});
    
    x = 5 * block_width + border_width; y = 10 * block_height + border_height;    
    labels[Labels::ShowNextPieceEnabled]->render(renderer, {x, y});
    
    x = 5 * block_width + border_width; y = 12 * block_height + border_height;    
    labels[Labels::Back]->render(renderer, {x, y});
}

void Render::renderInGameMenu() {
    int x, y;
    x = 5 * block_width + border_width; y = 8 * block_height + border_height;
    labels[Labels::Resume]->render(renderer, {x, y});

    x = 5 * block_width + border_width; y = 10 * block_height + border_height;
    labels[Labels::Exit]->render(renderer, {x, y});
}

void Render::renderCursor(unsigned int position, MenuType menu) {
    int x, y;
    SDL_Rect block;

    switch (menu) {
    case MenuType::MainMenu:
        x = 2 * block_width + border_width;
        y = (6 + position * 2) * block_height + border_height;               

        block = {x, y, block_width * 2, block_height * 2};   
        SDL_RenderCopy(renderer, cursor, NULL, &block);
        break;
    
    case MenuType::Settings:
        x = 2 * block_width + border_width;
        y = (6 + position * 2) * block_height + border_height;               

        block = {x, y, block_width, block_height};   
        SDL_RenderCopy(renderer, cursor, NULL, &block);
        break;

    case MenuType::InGameMenu:
        x = 2 * block_width + border_width;
        y = (8 + position * 2) * block_height + border_height;               

        block = {x, y, block_width * 2, block_height * 2};   
        SDL_RenderCopy(renderer, cursor, NULL, &block);
        break;
    }
}

void Render::renderBackground() {
    SDL_Rect background = {0, 0, window_width, window_height};
    SDL_RenderCopy(renderer, background_texture, NULL, &background);
}

void Render::renderBorders() {
    for (int row = 0; row < 22; row++) {
        for (int col = 0; col < 12; col++) {
            if (col > 0 && col < 11) {
                if (row == 0 || row == 21) {
                    SDL_Rect border_part = {col * border_width, row * border_height, border_width, border_height};
                    SDL_RenderCopy(renderer, borders_texture, NULL, &border_part);
                }
            }
            else {
                SDL_Rect border_part = {col * border_width, row * border_height, border_width, border_height};
                SDL_RenderCopy(renderer, borders_texture, NULL, &border_part);
            }
        }
    }
}

void Render::prerenderText() {
    labels[Labels::Next] = new Label(renderer, font, (char*)"Next piece:", grey);
    labels[Labels::Pause] = new Label(renderer, font_big, (char*)"Pause", white);
    labels[Labels::Holded] = new Label(renderer, font, (char*)"Holded Piece:", grey);
    labels[Labels::StartGame] = new Label(renderer, font_big, (char*)"Start game", grey);
    labels[Labels::Settings] = new Label(renderer, font_big, (char*)"Settings", grey);
    labels[Labels::HighScore] = new Label(renderer, font_big, (char*)"High score", grey);
    labels[Labels::Exit] = new Label(renderer, font_big, (char*)"Exit", grey);
    labels[Labels::GameEnded] = new Label(renderer, font_big, (char*)"GAME ENDED!!!", white);
    labels[Labels::ShadowEnabled] = new Label(renderer, font, (char*)"Shadow : ", grey);
    labels[Labels::HoldPieceEnabled] = new Label(renderer, font, (char*)"Hold piece : ", grey);
    labels[Labels::ShowNextPieceEnabled] = new Label(renderer, font, (char*)"Show next piece : ", grey);
    labels[Labels::Back] = new Label(renderer, font, (char*)"Back", grey);
    labels[Labels::On] = new Label(renderer, font, (char*)"On", green);
    labels[Labels::Off] = new Label(renderer, font, (char*)"Off", red);
    labels[Labels::Resume] = new Label(renderer, font_big, (char*)"Resume", grey);
    labels[Labels::Level] = new Label(renderer, font, (char*)"Level: ", grey);
    labels[Labels::Score] = new Label(renderer, font, (char*)"Score: ", grey);
}
 
void Render::renderText() {
    int x, y;
    x = 12 * block_width + border_width; y = 0 * block_height + border_height;
    labels[Labels::Next]->render(renderer, {x, y});
    
    x = 12 * block_width + border_width; y = 8 * block_height + border_height;
    labels[Labels::Level]->render(renderer, {x, y});
    dynamic_labels[DynamicLabels::LevelValue]->render(renderer, {x + labels[Labels::Level]->getSufaceSize().x, y});

    x = 12 * block_width + border_width; y = 10 * block_height + border_height;
    labels[Labels::Score]->render(renderer, {x, y});
    dynamic_labels[DynamicLabels::ScoreValue]->render(renderer, {x + labels[Labels::Score]->getSufaceSize().x, y});

    x = 12 * block_width + border_width; y = 15 * block_height + border_height;    
    labels[Labels::Holded]->render(renderer, {x, y});
}

void Render::renderGrid(const unsigned int grid[10][20]) {  
    for (int col = 0; col < 10; col++) {
        for (int row = 0; row < 20; row++) {
            if (grid[col][row]) {
                assert(grid[col][row] < 8);
                SDL_Rect block = {  border_width + block_width * col, 
                                    border_height + block_height * row, 
                                    block_width, 
                                    block_height};   
                SDL_RenderCopy(renderer, block_colors[grid[col][row]], NULL, &block);
            }
        }
    }
}

void Render::renderTetromino(const Tetromino& tetromino, bool in_grid_bounds, bool transparent) {
    if (tetromino.color == 0)
        return;

    for (int i = 0; i < 4; i++) {
        if (in_grid_bounds) {
            assert(tetromino.blocks[i][0] + tetromino.col >= 0 && tetromino.blocks[i][0] + tetromino.col < 10);
            assert(/*tetromino.blocks[i][1] + tetromino.row > 0 &&*/ tetromino.blocks[i][1] + tetromino.row < 20);
        }

        if (tetromino.blocks[i][1] + tetromino.row >= 0) {
            int x = (tetromino.blocks[i][0] + tetromino.col) * block_width + border_width;
            int y = (tetromino.blocks[i][1] + tetromino.row) * block_height + border_height;
    
            SDL_Rect block = {x, y, block_width, block_height};
        
            assert(tetromino.color < 9 && tetromino.color);
            if (transparent)
                SDL_SetTextureAlphaMod(block_colors[tetromino.color], 50);
            else  
                SDL_SetTextureAlphaMod(block_colors[tetromino.color], 255);
            
            SDL_RenderCopy(renderer, block_colors[tetromino.color], NULL, &block);
        }
    }
}


