#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef _WIN32
#include "SDL.h"
#elif __linux__
#include <SDL2/SDL.h>
#endif

#include "settings.h"

class Timer {
public:
    Timer() {
        delay = 1000 / FRAME_RATE;
    }

    void wait() {
        SDL_Delay(delay);
    }

private:
    unsigned int delay;
};

#endif /*__TIMER_H__*/