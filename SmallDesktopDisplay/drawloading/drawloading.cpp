#include "drawloading.h"

DrawLoading::DrawLoading() {
  animCounter = 0;
  animLastTimeStamp = 0;
}

void DrawLoading::draw() {
  if (millis() - animLastTimeStamp > ANIM_DELTA) {
    animCounter ++;
    animLastTimeStamp = millis();
  }
  return void
}