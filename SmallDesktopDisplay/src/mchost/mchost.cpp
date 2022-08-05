#include "mchost.h"

extern TFT_eSprite clk;
extern TFT_eSPI tft;

const int columnOuterWidth = 30;
const int columnOuterHeight = 180;
const int columnOuterRadius =  10;
const int columnOuterThickness = 3;
const int columnInnerWidth = 18;
const int columnInnerHeight = 168;
const int columnInnerRadius =  4;

const uint8_t *astImgArr[] = {
  ast0, ast4, ast8, ast12, ast16, ast19,
};

const uint32_t astImgSizeArr[] = {
  sizeof(ast0), sizeof(ast4), sizeof(ast8), sizeof(ast12), sizeof(ast16), sizeof(ast19),
};

void drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color, int32_t thickness) {
  for (int i = 0; i < thickness; i++) {
    clk.drawRoundRect(x + i, y + i, w - 2 * i, h - 2 * i, r - 2 * i, color);
  }
};

McHost::McHost(void) {
  hostInfo = HostInfo{ 0, 0, 0.00, 0.00 };
  timestampAst = 0;
  timestampInfo = 0;
};

void McHost::init() {
  // 清屏
  tft.fillScreen(TFT_BLACK);
  // cpu and mem layout
  clk.setColorDepth(8);
  clk.createSprite(columnOuterWidth, columnOuterHeight);
  clk.fillSprite(0x0000);
  drawRoundRect(0, 0, columnOuterWidth, columnOuterHeight, columnOuterRadius, TFT_WHITE, columnOuterThickness);
  clk.pushSprite(140, 20);
  clk.pushSprite(190, 20);
  clk.deleteSprite();
  TJpgDec.drawJpg(146, 210, icpu_20X20, sizeof(icpu_20X20)); //温度图标
  TJpgDec.drawJpg(196, 210, iDiagram_20X20, sizeof(iDiagram_20X20)); //湿度图标
}

void McHost::drawInfo() {
  // mock fetch data
  hostInfo.cpuData = (hostInfo.cpuData + 1) % 100;
  hostInfo.memData = (hostInfo.memData + 1) % 100;

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

void McHost::drawAst() {
  TJpgDec.drawJpg(20, 110, astImgArr[counterAst], astImgSizeArr[counterAst]);
  counterAst = (counterAst + 1) % 6;
};


void McHost::update() {
  int nowStamp = millis();

  if (nowStamp - timestampAst > TIME_GAP_AST) {
    drawAst();
    timestampAst = millis();
  } else if (nowStamp < timestampAst) { // 兼容milles极限
    timestampAst = millis();
  }

  if (nowStamp - timestampInfo > TIME_GAP_INFO) {
    drawInfo();
    timestampInfo = millis();
  } else if (nowStamp < timestampInfo) { // 兼容milles极限
    timestampInfo = millis();
  }
};