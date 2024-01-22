#ifndef __HIGH_SCORE_H__
#define __HIGH_SCORE_H__


#ifdef _WIN32
#include <json.h>
#elif __linux__
#include <json-c/json.h>
#endif

#include "render.h"

struct Record {
    char name[MAX_TEXT_CHARS];
    unsigned int score;
};

class HighScore {
public:
    HighScore();
    ~HighScore();

    void writeNewRecord(const char* name, unsigned int score);
    bool checkForNewRecord(unsigned int score);
    const Record* getRecords();
    
private:
    void load();
    void save();

    Record records[HIGH_SCORE_PLAYERS_NUMBER];
};


#endif /* __HIGH_SCORE_H__ */
