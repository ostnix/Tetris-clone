#include "high_score.h"

State HighScore::show(Render* _render, State _state) {
    Render* render = _render;
    State state = _state;
    if (render == NULL)
        return state;

    NamesList names_list;
    strcpy(names_list.name[0], "AAA");
    strcpy(names_list.name[1], "bbb");
    strcpy(names_list.name[2], "cc");
    strcpy(names_list.name[3], "dddd");
    strcpy(names_list.name[4], "ffffF");

    bool key_pressed = false;

    while (!key_pressed) {
        SDL_Delay(state.delay);
        //render->showHighScore(names_list);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN || event.type == SDL_QUIT) {
                key_pressed = true;
            }
        }
    }
    return state;
}

void HighScore::writeNewRecord(Render* _render) {

}