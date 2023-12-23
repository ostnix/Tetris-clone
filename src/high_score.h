#ifndef __HIGH_SCORE_H__
#define __HIGH_SCORE_H__

#include "settings.h"
#include "render.h"

class HighScore {
public:
    static State show(Render* _render, State _state);
    static void writeNewRecord(Render* _render);
private:
    HighScore();

};


#endif /* __HIGH_SCORE_H__ */
