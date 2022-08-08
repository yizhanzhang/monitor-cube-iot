#include "mcwallpaper.h"

McWallpaper::McWallpaper(void) {
  hasDraw = false;
}

void McWallpaper::init() {
  hasDraw = false;
}

void McWallpaper::update() {
  if (hasDraw) return void();
  hasDraw = true;
  TJpgDec.drawJpg(0, 0, windowsxp, sizeof(windowsxp));
}