#include "high_score.h"

HighScore::HighScore() {
    load();
}

HighScore::~HighScore() {
    //save();
}

void HighScore::writeNewRecord(const char* name, unsigned int score) {
    Record best_score;
    best_score.score = score;
    strcpy(best_score.name, name);

    for (int i = 0; i < HIGH_SCORE_PLAYERS_NUMBER; i++) {
        if (records[i].score < best_score.score) {
            Record temp;
            temp.score = records[i].score;
            strcpy(temp.name, records[i].name);

            records[i].score = best_score.score;
            strcpy(records[i].name, best_score.name);

            best_score.score = temp.score;
            strcpy(best_score.name, temp.name);
        }
    }

    save();
}

bool HighScore::checkForNewRecord(unsigned int score) {
    if (records[HIGH_SCORE_PLAYERS_NUMBER - 1].score < score)
        return true;

    return false;
}

const Record* HighScore::getRecords() {
    return records;
}

void HighScore::load() {
    json_object* root = json_object_from_file("HighScores.json");

    if (!root) {    // if no file exist, creating new table
        for (int i = 0; i < HIGH_SCORE_PLAYERS_NUMBER; i++) {
            strcpy(records[i].name, "EMPTY");
            records[i].score = 0;
        }
    }
    else {
        int n = json_object_array_length(root);
        if (n > HIGH_SCORE_PLAYERS_NUMBER)
            n = HIGH_SCORE_PLAYERS_NUMBER;

        for (int i = 0; i < n; i++) {
            json_object* player = json_object_array_get_idx(root, i);

            json_object* player_name = json_object_object_get(player, "Name");
            strcpy(records[i].name, json_object_get_string(player_name));

            json_object* player_score = json_object_object_get(player, "Score");
            records[i].score = json_object_get_int(player_score);
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
            json_object_object_add(player, "Name", json_object_new_string(records[i].name));
            json_object_object_add(player, "Score", json_object_new_int(records[i].score));
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
            json_object_set_string(player_name, records[i].name);

            json_object* player_score = json_object_object_get(player, "Score");
            json_object_set_int(player_score, records[i].score);

        }
    }
    assert(json_object_to_file_ext("HighScores.json", root, JSON_C_TO_STRING_PRETTY) >= 0);

    json_object_put(root);
}
