#include "mcwallpaper.h"

McWallpaper::McWallpaper(void) {
  hasDraw = false;
}

void McWallpaper::init() {
  hasDraw = false;
}

void McWallpaper::draw() {
  if (hasDraw) return void();
  hasDraw = true;
  TJpgDec.setJpgScale(2);
  TJpgDec.drawJpg(0, 0, windowsxp, sizeof(windowsxp));
  TJpgDec.setJpgScale(1);
}