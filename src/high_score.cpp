#include "high_score.h"

HighScore::HighScore(Render* _render) : render(_render) {
    load();
}

HighScore::~HighScore() {
    save();
}

void HighScore::show(State _state) {
    State state = _state;
    assert(render);

    bool key_pressed = false;

    render->clearLabels();
    render->addLabel(FontType::Big, "HIGH SCORES:", render->white, {4, 1});
    for (int i = 0; i < HIGH_SCORE_PLAYERS_NUMBER; i++) {
        render->addLabel(FontType::Normal, names_list[i].name, render->blue, {6, 5 + i * 2});
        render->addLabel(FontType::Normal, names_list[i].score, render->blue, {14, 5 + i * 2});
    }

    while (!key_pressed) {
        SDL_Delay(state.delay);
        render->renderOnlyLables();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN || event.type == SDL_QUIT) {
                key_pressed = true;
            }
        }
    }
}

void HighScore::checkForNewRecord(unsigned int score) {

}

void HighScore::load() {
    json_object* root = json_object_from_file("HighScores.json");

    if (!root) {    // if no file exist, creating new table
        for (int i = 0; i < HIGH_SCORE_PLAYERS_NUMBER; i++) {
            strcpy(names_list[i].name, "EMPTY");
            names_list[i].score = 0;
        }
    }
    else {
        int n = json_object_array_length(root);
        if (n > HIGH_SCORE_PLAYERS_NUMBER)
            n = HIGH_SCORE_PLAYERS_NUMBER;

        for (int i = 0; i < n; i++) {
            json_object* player = json_object_array_get_idx(root, i);

            json_object* player_name = json_object_object_get(player, "Name");
            strcpy(names_list[i].name, json_object_get_string(player_name));

            json_object* player_score = json_object_object_get(player, "Score");
            names_list[i].score = json_object_get_int(player_score);
        }
    }

    json_object_put(root);
}

void HighScore::save() {
    json_object* root = json_object_from_file("HighScores.json");

    if (!root) {
        root = json_object_new_array_ext(HIGH_SCORE_PLAYERS_NUMBER);
        for (int i = 0; i < HIGH_SCORE_PLAYERS_NUMBER; i++) {
            json_object* player = json_object_new_object();
            json_object_object_add(player, "Name", json_object_new_string(names_list[i].name));
            json_object_object_add(player, "Score", json_object_new_int(names_list[i].score));
            json_object_array_add(root, player);
        }
    }
    else {
        int n = json_object_array_length(root);
        if (n > HIGH_SCORE_PLAYERS_NUMBER)
            n = HIGH_SCORE_PLAYERS_NUMBER;

        for (int i = 0; i < n; i++) {
            json_object* player = json_object_array_get_idx(root, i);

            json_object* player_name = json_object_object_get(player, "Name");
            json_object_set_string(player_name, names_list[i].name);

            json_object* player_score = json_object_object_get(player, "Score");
            json_object_set_int(player_score, names_list[i].score);

        }
    }
    assert(json_object_to_file_ext("HighScores.json", root, JSON_C_TO_STRING_PRETTY) >= 0);

    json_object_put(root);
}