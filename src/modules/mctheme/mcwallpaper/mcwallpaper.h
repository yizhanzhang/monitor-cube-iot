#ifndef MC_WALLPAPER_H
#define MC_WALLPAPER_H

#include <Arduino.h>
#include <FS.h>
#include <JPEGDecoder.h>
#include "jpg/jpg.h"
#include "../base.h"

class McWallpaper: public McThemeBase {
  private:
    int counter;
    int fileAmount;
    void getFileAmount();
    void listFiles();
    void drawFile();
  public:
    McWallpaper();
    void init();
    void update();
    void click();
};

#endif