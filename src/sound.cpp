#include "sound.h"

Sound::Sound() {
    SDL_Init(SDL_INIT_AUDIO);
}

Sound::~Sound() {
    Mix_CloseAudio();
    Mix_Quit();
}

void Sound::playSound(SoundEffect effect) {
    if (effect == SoundEffect::None)
        return;

    Mix_PlayChannel(-1, sounds[(unsigned int)effect], 0);
}

bool Sound::loadSounds() {
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048))
        return false;

    sounds[1] = Mix_LoadWAV("./sound/menu_select.wav");
    sounds[2] = Mix_LoadWAV("./sound/menu_move.wav");
    sounds[3] = Mix_LoadWAV("./sound/wrong_move.wav");
    sounds[4] = Mix_LoadWAV("./sound/typing.wav");
    sounds[5] = Mix_LoadWAV("./sound/new_high_score.wav");
    sounds[6] = Mix_LoadWAV("./sound/game_end.wav");
    sounds[7] = Mix_LoadWAV("./sound/fast_drop.wav");
    sounds[8] = Mix_LoadWAV("./sound/line_cleared.wav");
    sounds[9] = Mix_LoadWAV("./sound/four_lines_cleared.wav");
    sounds[10] = Mix_LoadWAV("./sound/hold.wav");
    
    bool result = true;

    for (int i = 1; i < 11; i++) {
        result = result && sounds[i];
    }
    
    return result;
}