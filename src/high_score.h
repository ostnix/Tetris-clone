#ifndef __HIGH_SCORE_H__
#define __HIGH_SCORE_H__

#include <json.h>
#include "settings.h"
#include "render.h"

struct RecordsList {
    char name[MAX_TEXT_CHARS];
    unsigned int score;
};

class HighScore {
public:
    HighScore(Render* _render);
    ~HighScore();

    void show(State _state);
    void checkForNewRecord(State& state);

private:
    void writeNewRecord(State& state);

    void load();
    void save();

    RecordsList records_list[HIGH_SCORE_PLAYERS_NUMBER];
    Render* render;
;
};


#endif /* __HIGH_SCORE_H__ */
