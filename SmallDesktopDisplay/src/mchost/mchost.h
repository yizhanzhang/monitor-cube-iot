#ifndef MC_HOST_H
#define MC_HOST_H

#include <TFT_eSPI.h>

class McHost {
  public:
    void init();
    void update();
  private:
    void fetch();
    void draw();
};

# endif