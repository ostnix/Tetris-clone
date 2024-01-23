#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include <dirent.h>

#include "settings.h"
#include "tetromino.h"
#include "render.h"
#include "high_score.h"

class GameView {
public:
    GameView(int width, int height);
    ~GameView();

    void draw(State& state);
    void updateGrid(State& state);
    void updateScreen(State& state);
    
    bool tryLoadTexturePack(unsigned int pack_index);
    unsigned int getNumberOfTexturePacks();

private:
    void redrawBackground();
    void showPause();
    void createTexts();
    void putTetromino(Tetromino& tetromino, LayerId id);
    void drawGame(State& state);
    void drawIngameMenu(State& state);
    void drawMainMenu(State& state);
    void drawSettings(State& state);
    void drawHighScore(State& state);
    void drawTexturePacksList(State& state);
    void drawNewHighScore(State& state);
    void getPacksList();

    enum Texts {Next = 0, Holded = 1, Pause = 2, Score = 3, Level = 4, ScoreVal = 5, 
                LevelVal = 6, NewGame = 7, Settings = 8, HighScore = 9, Exit = 10, 
                Resume = 11, MainMenu = 12, On = 13, Off = 14, Shadow = 15, 
                Back = 16, TexturePacks = 17, CantLoadTexture = 18, NewHighScore = 19, EnterName = 20, GameEnd = 21};

    LayerId game_view = -1;
    LayerId game_grid = -1;
    LayerId ingame_menu = -1;
    LayerId main_menu = -1;

    TextId player_name = -1;
    TextId records_text[HIGH_SCORE_PLAYERS_NUMBER] = {};
    TextId texts[MAX_TEXTS] = {};
    TextId texture_pack_folders[MAX_TEXTURE_PACKS];

    char pack_folders_names[MAX_TEXTURE_PACKS][MAX_TEXT_CHARS];
    unsigned int number_of_texture_packs = 0;

    Render* render;
};


#endif /* __GAME_VIEW_H__ */
