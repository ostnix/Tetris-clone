#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "tetromino.h"
#include "high_score.h"

static const unsigned int FRAME_RATE = 60;

const int GAME_FIELD_WIDTH = 10;
const int GAME_FIELD_HEIGHT = 20;

const unsigned int MAX_NUM_OF_TETROMINOES = 4;

static const unsigned int NEXT = 3;
static const unsigned int ACTIVE = 2;
static const unsigned int HOLDED = 1;
static const unsigned int SHADOW = 0;

enum class MenuType {MainMenu, Settings, InGameMenu, HighScore, Game, TexturePacks};
enum class PlayerAction {None, Down, Up, Left, Right, FastDrop, HoldPiece, Exit, Pause, ESC, Enter, BackSpace, SymbolInput, Action, OtherKey};

struct Settings {
};

struct State {
    // game state
    int score = 0;
    unsigned int level = 0;
    unsigned char cursor_position = 0;

    int delay = 0;

    bool quit_game = false;
    bool game_ended = false;
    bool rerender = true;
    bool paused = false;
    bool soft_drop = false;
    bool instant_drop = false;
    bool update_score = false;
    bool update_level = false;
    bool new_high_score = false;
    bool enter_player_name = false;
    
    PlayerAction last_player_action = PlayerAction::None;
    MenuType context = MenuType::MainMenu;

    Cell grid[GAME_FIELD_WIDTH][GAME_FIELD_HEIGHT];

    Tetromino tetrominoes[MAX_NUM_OF_TETROMINOES];

    char player_name[MAX_TEXT_CHARS];

    const Record* records;

    // settings
    bool shadow_enabled = true;
    bool show_next_piece_enabled = true;
    bool hold_enabled = true;
    bool soft_drop_enabled = true;
    bool soft_drop_instant_blocking = false;

    char path_to_textures[40] = "./textures/default/";
};

#endif /* __SETTINGS_H__ */ 
