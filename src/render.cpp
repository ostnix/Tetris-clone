#include "render.h"

Render::Render(int width, int height) : window_width(width), window_height(height) {
    TTF_Init();

    assert(SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window, &renderer) == 0);

    assert(loadTextures());
    assert(loadFonts());
    //prerenderText();

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
    //renderText();
    renderGrid(grid);
    renderTetromino(shadow_tetromino, true, true);
    renderTetromino(tetromino, true, false); 
    renderTetromino(next_tetromino, false, false);
    renderTetromino(holded_tetromino, false, false);
    renderLabels();
    SDL_RenderPresent(renderer);
}

int Render::addLabel(FontType font_type, unsigned int number, SDL_Color color, SDL_Point point) {
    char text[MAX_TEXT_CHARS + sizeof(char)];
    sprintf(text, "%d", number);
    return addLabel(font_type, text, color, point);
}

int Render::addLabel(FontType font_type, const char* text, SDL_Color color, SDL_Point point) {
    assert(strlen(text) < MAX_TEXT_CHARS);
    assert(num_of_added_labels < MAXIMUM_NUM_OF_LABELS);

    switch (font_type) {
    case FontType::Normal:
        labels[num_of_added_labels] = new Label(renderer, font, text, color, {point.x * block_width + border_width,
                                                                              point.y * block_height + border_height});
        break;

    case FontType::Big:
        labels[num_of_added_labels] = new Label(renderer, font_big, text, color, { point.x * block_width + border_width,
                                                                                   point.y * block_height + border_height});
        break;
    }

    num_of_added_labels++;
    return (num_of_added_labels - 1);
}

void Render::updateLabel(unsigned int number, unsigned int label_index) {
    char text[MAX_TEXT_CHARS + sizeof(char)];
    sprintf(text, "%d", number);
    updateLabel(text, label_index);
}

void Render::updateLabel(const char* text, unsigned int label_index) {
    assert(strlen(text) < MAX_TEXT_CHARS);
    assert(label_index < num_of_added_labels);

    labels[label_index]->updateText(renderer, text);

}

void Render::updateLabel(const char* text, unsigned int label_index, SDL_Color color) {
    assert(strlen(text) < MAX_TEXT_CHARS);
    assert(label_index < num_of_added_labels);

    labels[label_index]->updateText(renderer, text, color);
}

void Render::popupLabel(FontType font_type, const char* text, SDL_Color color, SDL_Point point) {
    TTF_Font* _font = font;
    
    if (font_type == FontType::Big)
        _font = font_big;
    
    Label message(renderer, _font, text, color, {point.x * block_width + border_width,
                                    point.y * block_height + border_height});
    message.render(renderer);
    SDL_RenderPresent(renderer);
}

void Render::renderOnlyLables() {
    SDL_RenderClear(renderer);
    renderBackground();
    renderLabels();
    SDL_RenderPresent(renderer);
}

void Render::clearLabels() {
    for (int i = 0; i < num_of_added_labels; i++) {
        delete labels[i];
    }

    num_of_added_labels = 0;
}

void Render::showCursor(SDL_Point point) {
    SDL_Rect block = {point.x * block_width + border_width, point.y * block_height + border_height, block_width * 2, block_height * 2};
    SDL_RenderCopy(renderer, cursor, NULL, &block);
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

void Render::renderLabels() {
    for (int i = 0; i < num_of_added_labels; i++) {
        labels[i]->render(renderer);
    }
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


