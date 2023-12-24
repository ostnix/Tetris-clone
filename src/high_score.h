#ifndef __HIGH_SCORE_H__
#define __HIGH_SCORE_H__

#include "settings.h"
#include "render.h"

class HighScore {
public:
    static State show(Render* _render, State _state);
    static void writeNewRecord(Render* _render);
    static void getTable();

private: 
    static NamesList loadTable();
    static bool saveTable(NamesList names_list);
    static bool updateTable(NamesList names_list);

    HighScore();
};


#endif /* __HIGH_SCORE_H__ */
