#ifndef __INPUT_H__
#define __INPUT_H__

#ifdef _WIN32
    #include "SDL.h"
#elif __linux__
    #include <SDL2/SDL.h>
#endif

#include "settings.h"

class Input {
public:
    Input() {}
    ~Input() {}

    PlayerAction getEvent();
    void keyboardTextInput(bool active = true);

private:
};

#endif /*__INPUT_H__*/