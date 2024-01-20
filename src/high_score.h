#ifndef __HIGH_SCORE_H__
#define __HIGH_SCORE_H__


#ifdef _WIN32
#include <json.h>
#elif __linux__
#include <json-c/json.h>
#endif

#include "settings.h"
#include "render.h"

struct Record {
    char name[MAX_TEXT_CHARS];
    unsigned int score;
};

class HighScore {
public:
    HighScore(Render* _render);
    ~HighScore();

    bool checkForNewRecord(State& state);
    

private:
    void load();
    void save();

    Record records_list[HIGH_SCORE_PLAYERS_NUMBER];
    Render* render;
};


#endif /* __HIGH_SCORE_H__ */
