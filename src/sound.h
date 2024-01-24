#ifndef __SOUND_H__
#define __SOUND_H__

#ifdef _WIN32
#include "SDL.h"
#include "SDL_mixer.h"
#elif __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif


static const unsigned int MAX_SOUNDS = 10;
enum class SoundEffect {None = 0, MenuSelect = 1 , MenuMove = 2, WrongMove = 3, Typing = 4, NewHighScore =5, GameEnd = 6, FastDrop = 7, LineCleared = 8, FourLinesCleared = 9};

class Sound {
public:
    Sound();
    ~Sound();

    void playSound(SoundEffect effect);

    bool loadSounds();

private:
    Mix_Chunk* sounds[MAX_SOUNDS] = {};
};



#endif /*__SOUND_H__*/