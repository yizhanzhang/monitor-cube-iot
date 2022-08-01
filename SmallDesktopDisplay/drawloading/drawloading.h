#ifndef DRAW_LOADING_H
#define DRAW_LOADING_H

#include <TimeLib.h>

#define ANIM_DELTA 30

class DrawLoading {
  private:
    int animCounter;
    int animLastTimeStamp;
  public:
    DrawLoading();
    void draw();
};

#endif