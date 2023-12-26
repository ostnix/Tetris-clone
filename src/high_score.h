#ifndef __HIGH_SCORE_H__
#define __HIGH_SCORE_H__

#include <json.h>
#include "settings.h"
#include "render.h"

struct NamesList {
    char name[MAX_TEXT_CHARS];
    unsigned int score;
};

class HighScore {
public:
    HighScore(Render* _render);
    ~HighScore();

    void show(State _state);
    void checkForNewRecord(unsigned int score);

private:
    void load();
    void save();

    NamesList names_list[HIGH_SCORE_PLAYERS_NUMBER];
    Render* render;
;
};


#endif /* __HIGH_SCORE_H__ */
