#ifndef __RENDER_H__
#define __RENDER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cassert>

#ifdef _WIN32
    #include "SDL.h"
    #include "SDL_image.h"
    #include "SDL_ttf.h"
#elif __linux__
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
#endif

#include "tetromino.h"

enum class FontType {Normal, Big};

static const unsigned int HIGH_SCORE_PLAYERS_NUMBER = 5;
static const unsigned int MAXIMUM_NUM_OF_LABELS = 10;
static const unsigned int MAX_TEXT_CHARS = 15;
static const unsigned int BORDER_SIZE = 60;
static const unsigned int BLOCK_SIZE = 60;
static const unsigned int MAX_COLORS = 9;
static const unsigned int BLOCKED_COLOR = 8;

struct NamesList {
    char name[HIGH_SCORE_PLAYERS_NUMBER][MAX_TEXT_CHARS];
};

class Render {
public:
    Render(int width, int height);
    ~Render();

    void renderGame(const unsigned int grid[10][20], 
                    const Tetromino& tetromino, 
                    const Tetromino& next_tetromino, 
                    const Tetromino& holded_tetromino,
                    const Tetromino& shadow_tetromino);
    
    int addLabel(FontType font_type, unsigned int number, SDL_Color color, SDL_Point point);
    int addLabel(FontType font_type, const char* text, SDL_Color color, SDL_Point point);
    void popupLabel(FontType font_type, const char* text, SDL_Color color, SDL_Point point);
    void updateLabel(unsigned int number, unsigned int label_index);
    void updateLabel(const char* text, unsigned int label_index);
    void updateLabel(const char* text, unsigned int label_index, SDL_Color color);
    void renderOnlyLables();
    void clearLabels();
    void showCursor(SDL_Point point);

    const SDL_Color gray = {200, 200, 200};
    const SDL_Color white = {250, 250, 250};
    const SDL_Color red = {250, 50, 50};
    const SDL_Color green = {50, 250, 50};
    const SDL_Color blue = {50, 50, 250};

private:
    struct Label {
        Label(SDL_Renderer* _renderer, TTF_Font* _font, const char* _text, SDL_Color _color) : font(_font), color(_color) {
            surface = TTF_RenderUTF8_Solid(font, _text, _color);
            texture = SDL_CreateTextureFromSurface(_renderer, surface);
        }

        Label(SDL_Renderer* _renderer, TTF_Font* _font, const char* _text, SDL_Color _color, SDL_Point _point) : Label(_renderer, _font, _text, _color) {
            point = _point;
        }

        ~Label() {
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }

        void updateText(SDL_Renderer* _renderer, const char* _text) {
            updateText(_renderer, font, _text, color);
        }

        void updateText(SDL_Renderer* _renderer, const char* _text, SDL_Color _color) {
            updateText(_renderer, font, _text, _color);
        }

        void updateText(SDL_Renderer* _renderer, TTF_Font* _font, const char* _text, SDL_Color _color) {
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);           
            
            surface = TTF_RenderUTF8_Solid(_font, _text, _color); 
            texture = SDL_CreateTextureFromSurface(_renderer, surface);
        }

        void render(SDL_Renderer* _renderer) {
            SDL_Rect rect = {point.x, point.y, surface->w, surface->h};
            SDL_RenderCopy(_renderer, texture, NULL, &rect);
        }

        void render(SDL_Renderer* _renderer, SDL_Point p) {
            SDL_Rect rect = {p.x, p.y, surface->w, surface->h};
            SDL_RenderCopy(_renderer, texture, NULL, &rect);
        }

        TTF_Font* font; 
        SDL_Color color;

        SDL_Point point = {0, 0};
        SDL_Point getSufaceSize() {return {surface->w, surface->h};}

        SDL_Surface* surface;
        SDL_Texture* texture;
    };

    bool loadTextures();
    bool loadFonts();

    void renderBackground();
    void renderBorders();
    void renderLabels();
    
    void renderGrid(const unsigned int grid[10][20]);
    void renderTetromino(const Tetromino& tetromino, bool in_grid_bounds, bool transparent);

    int window_width;
    int window_height;
    int block_width;
    int block_height;
    int border_width;
    int border_height;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* background_texture;
    SDL_Texture* borders_texture;
    SDL_Texture* block_colors[MAX_COLORS];
    SDL_Texture* cursor;

    Label* labels[MAXIMUM_NUM_OF_LABELS] = {NULL};
    unsigned int num_of_added_labels = 0;

    TTF_Font* font; 
    TTF_Font* font_big;


};

#endif /* __RENDER_H__ */
