#ifndef MC_WALLPAPER_H
#define MC_WALLPAPER_H

#include <TJpg_Decoder.h>
#include "../../img/wallpaper/windows-xp-x64.h"

class McWallpaper {
  private:
    bool hasDraw;
  public:
    McWallpaper();
    void init();
    void draw();
};

#endif