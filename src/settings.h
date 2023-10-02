#ifndef __SETTINGS_H__
#define __SETTINGS_H__

static const unsigned int FRAME_RATE = 60;


struct Settings {
};

struct State {
    // game state
    unsigned int score = 0;
    unsigned int level = 0;
    unsigned int delay = 0;
    unsigned int backup_color = 1;
    unsigned int cursor_position = 0;

    bool quit_game = false;
    bool game_ended = false;
    bool should_be_placed = false;
    bool rerender = true;
    bool paused = false;
    bool soft_drop = false;
    
    // settings
    bool shadow_enabled = true;
    bool show_next_piece_enabled = true;
    bool hold_piece_enabled = true;
    bool soft_drop_enabled = true;

    // TODO: Custom path to textures.
};

#endif /* __SETTINGS_H__ */ 
