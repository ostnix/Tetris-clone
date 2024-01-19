#include "high_score.h"

HighScore::HighScore(Render* _render) : render(_render) {
    load();
}

HighScore::~HighScore() {
    save();
}

bool HighScore::checkForNewRecord(State& state) {
    return false;
}

/*
const RecordsList& HighScore::getRecords() {
    return records_list;
}
*/
/*
void HighScore::show(State _state) {
    State state = _state;
    assert(render);

    bool key_pressed = false;

    render->clearLabels();
    render->addLabel(FontType::Big, "HIGH SCORES:", render->white, {4, 1});
    for (int i = 0; i < HIGH_SCORE_PLAYERS_NUMBER; i++) {
        render->addLabel(FontType::Normal, records_list[i].name, render->blue, {6, 5 + i * 2});
        render->addLabel(FontType::Normal, records_list[i].score, render->blue, {14, 5 + i * 2});
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

void HighScore::checkForNewRecord(State& state) {
    //if (records_list[HIGH_SCORE_PLAYERS_NUMBER - 1].score < state.score)
    //    writeNewRecord(state);
}
*/
/*
void HighScore::writeNewRecord(State& state) {

    char player_name[MAX_TEXT_CHARS] = {'\0'};

    render->clearLabels();
    render->addLabel(FontType::Big, "New Record!!!", render->gray, {5, 1});
    render->addLabel(FontType::Normal, "Ender name:", render->gray, {1, 4});
    LabelId text_edit = render->addLabel(FontType::Normal, player_name, render->red, {5, 6});

    bool enter_pressed = false;

    SDL_StartTextInput();
    while (!enter_pressed) {
        SDL_Delay(state.delay);
        render->renderOnlyLables();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                state.quit_game = true;
                return;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    state.quit_game = true;
                    return;
                    break;

                case SDLK_BACKSPACE:
                    if (strlen(player_name) > 0) {
                        player_name[strlen(player_name) - 1] = '\0';
                        render->updateLabel(player_name, text_edit);
                    }
                    break;

                case SDLK_RETURN:
                    enter_pressed = true;
                    break;
                }

            case SDL_TEXTINPUT:
                if ((strlen(player_name) + strlen(event.text.text) < MAX_TEXT_CHARS) && (event.text.text[0] != '\x1')) {
                    strcat(player_name, event.text.text);
                    render->updateLabel(player_name, text_edit);
                }
                break;
            }
        }
    }
    SDL_StopTextInput();

    RecordsList best_score;
    best_score.score = state.score;
    strcpy(best_score.name, player_name);

    for (int i = 0; i < HIGH_SCORE_PLAYERS_NUMBER; i++) {
        if (records_list[i].score < best_score.score) {
            RecordsList temp;
            temp.score = records_list[i].score;
            strcpy(temp.name, records_list[i].name);

            records_list[i].score = best_score.score;
            strcpy(records_list[i].name, best_score.name);

            best_score.score = temp.score;
            strcpy(best_score.name, temp.name);
        }
    }
    
}
*/

void HighScore::load() {
    json_object* root = json_object_from_file("HighScores.json");

    if (!root) {    // if no file exist, creating new table
        for (int i = 0; i < HIGH_SCORE_PLAYERS_NUMBER; i++) {
            strcpy(records_list[i].name, "EMPTY");
            records_list[i].score = 0;
        }
    }
    else {
        int n = json_object_array_length(root);
        if (n > HIGH_SCORE_PLAYERS_NUMBER)
            n = HIGH_SCORE_PLAYERS_NUMBER;

        for (int i = 0; i < n; i++) {
            json_object* player = json_object_array_get_idx(root, i);

            json_object* player_name = json_object_object_get(player, "Name");
            strcpy(records_list[i].name, json_object_get_string(player_name));

            json_object* player_score = json_object_object_get(player, "Score");
            records_list[i].score = json_object_get_int(player_score);
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
            json_object_object_add(player, "Name", json_object_new_string(records_list[i].name));
            json_object_object_add(player, "Score", json_object_new_int(records_list[i].score));
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
            json_object_set_string(player_name, records_list[i].name);

            json_object* player_score = json_object_object_get(player, "Score");
            json_object_set_int(player_score, records_list[i].score);

        }
    }
    assert(json_object_to_file_ext("HighScores.json", root, JSON_C_TO_STRING_PRETTY) >= 0);

    json_object_put(root);
}
