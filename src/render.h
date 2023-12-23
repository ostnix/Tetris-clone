#ifndef __RENDER_H__
#define __RENDER_H__

#include <stdio.h>
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

enum MenuType {MainMenu, Settings, InGameMenu};

static const unsigned int MAX_DIGITS = 10;
static const unsigned int BORDER_SIZE = 60;
static const unsigned int BLOCK_SIZE = 60;
static const unsigned int MAX_COLORS = 9;
static const unsigned int BLOCKED_COLOR = 8;

class Render {
public:
    Render(int width, int height);
    ~Render();

    void renderGame(const unsigned int grid[10][20], 
                    const Tetromino& tetromino, 
                    const Tetromino& next_tetromino, 
                    const Tetromino& holded_tetromino,
                    const Tetromino& shadow_tetromino);
    
    void showPause();
    void prerenderScoreAndLevel(int score, int level);

    void showHighScore();
    void showMenu(MenuType menu, unsigned int cursor_position);
    void showGameEnded();

private:
    struct Label {
        Label(SDL_Renderer* _renderer, TTF_Font* _font, const char* _text, SDL_Color _color) {
            surface = TTF_RenderUTF8_Solid(_font, _text, _color); 
            texture = SDL_CreateTextureFromSurface(_renderer, surface);
        }
        
        ~Label() {
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
    
        void updateText(SDL_Renderer* _renderer, TTF_Font* _font, const char* _text, SDL_Color _color) {
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);           
            
            surface = TTF_RenderUTF8_Solid(_font, _text, _color); 
            texture = SDL_CreateTextureFromSurface(_renderer, surface);
        }

        void render(SDL_Renderer* _renderer, SDL_Point p) {
            SDL_Rect rect = {p.x, p.y, surface->w, surface->h};
            SDL_RenderCopy(_renderer, texture, NULL, &rect);
        }

        SDL_Point getSufaceSize() {return {surface->w, surface->h};}

        SDL_Surface* surface;
        SDL_Texture* texture;
    };

    bool loadTextures();
    bool loadFonts();

    void renderMainMenu();
    void renderSettings();
    void renderInGameMenu();
    void renderCursor(unsigned int position, MenuType menu);
    void renderBackground();
    void renderBorders();
    void prerenderText();
    void renderText();
    void generateStaticTextTexture();
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

    enum Labels {
        None = 0,
        Next = 1,
        Pause = 2,
        Level = 3,
        Score = 4,
        Holded = 5,
        StartGame = 6,
        Settings = 7,
        HighScore = 8,
        Exit = 9,
        On = 10,
        Off = 11,
        GameEnded = 12,
        ShadowEnabled = 13,
        HoldPieceEnabled = 14,
        ShowNextPieceEnabled = 15,
        Back = 16, 
        Resume = 17
    };

    enum DynamicLabels {
        LevelValue = 0,
        ScoreValue = 1
    };

    Label* labels[18] = {NULL};
    Label* dynamic_labels[2] = {NULL};

    TTF_Font* font; 
    TTF_Font* font_big;

    SDL_Color grey = {200, 200, 200};
    SDL_Color white = {250, 250, 250};
    SDL_Color red = {250, 50, 50};
    SDL_Color green = {50, 250, 50};
};

#endif /* __RENDER_H__ */
