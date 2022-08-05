#include "mchost.h"

extern TFT_eSprite clk;

void drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color, int32_t thickness) {
  for (int i = 0; i < thickness; i++) {
    clk.drawRoundRect(x + i, y + i, w - 2 * i, h - 2 * i, r - 2 * i, color);
  }
}

void McHost::fetch() {

};

void McHost::init() {
  Serial.println("init host");
  clk.setColorDepth(8);
  clk.createSprite(240, 240);
  clk.fillSprite(0x0000);
  drawRoundRect(140, 20, 30, 180, 10, TFT_CYAN, 3);
  drawRoundRect(140, 20, 30, 180, 10, TFT_CYAN, 3);
  clk.fillRect(0, 0, 100, 100, TFT_RED);
  Serial.println("pushSprite");
  clk.pushSprite(0, 0);
  clk.deleteSprite();
}

void McHost::draw() {
  // 初始化彩屏
  clk.setColorDepth(8);
  clk.createSprite(200, 130);//创建窗口
  clk.fillSprite(0x0000);
  clk.drawRoundRect(0, 10, 50, 100, 10, TFT_GREEN);
  clk.pushSprite(20, 130);
  clk.deleteSprite();
};


void McHost::update() {
  fetch();
  draw();
};