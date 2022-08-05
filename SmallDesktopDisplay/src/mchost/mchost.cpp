#include "mchost.h"

extern TFT_eSprite clk;
extern TFT_eSPI tft;

int columnOuterWidth = 30;
int columnOuterHeight = 180;
int columnOuterRadius =  10;
int columnOuterThickness = 3;

int columnInnerWidth = 18;
int columnInnerHeight = 168;
int columnInnerRadius =  4;

void drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color, int32_t thickness) {
  for (int i = 0; i < thickness; i++) {
    clk.drawRoundRect(x + i, y + i, w - 2 * i, h - 2 * i, r - 2 * i, color);
  }
}

McHost::McHost(void) {
  hostInfo = HostInfo{ 0, 0, 0.00, 0.00 };
};

void McHost::init() {
  // 清屏
  tft.fillScreen(TFT_BLACK);
  // cpu and mem
  clk.setColorDepth(8);
  clk.createSprite(120, 240);
  clk.fillSprite(0x0000);
  drawRoundRect(20, 20, columnOuterWidth, columnOuterHeight, columnOuterRadius, TFT_CYAN, columnOuterThickness);
  drawRoundRect(70, 20, columnOuterWidth, columnOuterHeight, columnOuterRadius, TFT_GREEN, columnOuterThickness);
  clk.pushSprite(120, 0);
  clk.deleteSprite();
  TJpgDec.drawJpg(146, 210, icpu_20X20, sizeof(icpu_20X20)); //温度图标
  TJpgDec.drawJpg(196, 210, iDiagram_20X20, sizeof(iDiagram_20X20)); //湿度图标
}

void McHost::fetch() {
  hostInfo.cpuData = (hostInfo.cpuData + 1) % 100;
  hostInfo.memData = (hostInfo.memData + 1) % 100;
};

void McHost::draw() {
  // fill cpu percent
  clk.setColorDepth(8);
  clk.createSprite(columnInnerWidth, columnInnerHeight);
  clk.fillSprite(0x0000);
  int cpuY = hostInfo.cpuData * columnInnerHeight / 100;
  clk.fillRoundRect(0, columnInnerHeight - cpuY, columnInnerWidth, cpuY, columnInnerRadius, TFT_CYAN);
  clk.pushSprite(146, 26);
  clk.deleteSprite();
  // fill mem percent
  clk.setColorDepth(8);
  clk.createSprite(columnInnerWidth, columnInnerHeight);
  clk.fillSprite(0x0000);
  int memY = hostInfo.memData * columnInnerHeight / 100;
  clk.fillRoundRect(0, columnInnerHeight - memY, columnInnerWidth, memY, columnInnerRadius, TFT_GREEN);
  clk.pushSprite(196, 26);
  clk.deleteSprite();
};


void McHost::update() {
  fetch();
  draw();
};