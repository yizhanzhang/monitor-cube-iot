#include "mchost.h"

extern TFT_eSprite clk;
extern TFT_eSPI tft;

StaticJsonDocument<200> doc;

const int columnOuterWidth = 30;
const int columnOuterHeight = 180;
const int columnOuterRadius =  10;
const int columnOuterThickness = 3;
const int columnInnerWidth = 18;
const int columnInnerHeight = 168;
const int columnInnerRadius =  4;

const uint8_t *astImgArr[] = {
  ast0,  ast4,  ast8,  ast12,  ast16,
  ast20, ast24, ast28, ast32, ast36,
};

uint32_t astImgSizeArr[] = {
  sizeof(ast0),  sizeof(ast4),  sizeof(ast8),  sizeof(ast12),  sizeof(ast16),
  sizeof(ast20), sizeof(ast24), sizeof(ast28), sizeof(ast32), sizeof(ast36),
};

int astImgArrSize = sizeof(astImgArr) / sizeof(astImgArr[0]);

void drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color, int32_t thickness) {
  for (int i = 0; i < thickness; i++) {
    clk.drawRoundRect(x + i, y + i, w - 2 * i, h - 2 * i, r - 2 * i, color);
  }
};

McHost::McHost(void) {
  hostInfo = HostInfo{ 0, 0, "00.00M", "00.00M" };
  timestampInfo = 0;
  timestampAst = 0;
  counterAst = 0;
};

void McHost::init() {
  Serial.println("McHost::init");
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
  TJpgDec.drawJpg(146, 210, icpu_20X20, sizeof(icpu_20X20));
  TJpgDec.drawJpg(196, 210, imem_20X20, sizeof(imem_20X20));
  // net layout
  TJpgDec.drawJpg(20, 35, iupload_20X20, sizeof(iupload_20X20));
  TJpgDec.drawJpg(20, 70, idownload_20X20, sizeof(idownload_20X20));
  // drawAst
  drawAst();
};

void McHost::drawAst() {
  TJpgDec.drawJpg(20, 110, astImgArr[counterAst], astImgSizeArr[counterAst]);
  counterAst = (counterAst + 1) % astImgArrSize;
};

void McHost::update() {
  int nowStamp = millis();

  if (nowStamp - timestampInfo > TIME_GAP_INFO) {
    drawInfo();
    timestampInfo = millis();
  }
  if (nowStamp - timestampAst > TIME_GAP_AST) {
    drawAst();
    timestampAst = millis();
  }

  // 兼容milles极限
  if (nowStamp < timestampInfo) { 
    timestampInfo = millis();
  }
  if (nowStamp < timestampAst) {
    timestampAst = millis();
  }
};

void McHost::drawInfo() {
  // fill cpu percent
  clk.setColorDepth(8);
  clk.createSprite(columnInnerWidth, columnInnerHeight);
  clk.fillSprite(0x0000);
  if (hostInfo.cpuData > 0) {
    int cpuY = hostInfo.cpuData * columnInnerHeight / 100;
    clk.fillRoundRect(0, columnInnerHeight - cpuY, columnInnerWidth, cpuY, columnInnerRadius, TFT_CYAN);
  }
  clk.pushSprite(146, 26);
  clk.deleteSprite();
  // fill mem percent
  clk.setColorDepth(8);
  clk.createSprite(columnInnerWidth, columnInnerHeight);
  clk.fillSprite(0x0000);
  if (hostInfo.memData) {
    int memY = hostInfo.memData * columnInnerHeight / 100;
    clk.fillRoundRect(0, columnInnerHeight - memY, columnInnerWidth, memY, columnInnerRadius, TFT_GREEN);
  }
  clk.pushSprite(196, 26);
  clk.deleteSprite();
  // write net upload
  clk.setColorDepth(8);
  clk.createSprite(90, 24);
  clk.fillSprite(0x0000);
  clk.setTextSize(2);
  clk.setTextColor(TFT_ORANGE);
  clk.setTextDatum(CC_DATUM);
  clk.drawString(hostInfo.netUploadData, 45, 12);
  clk.pushSprite(47, 69);
  // write net download
  clk.setColorDepth(8);
  clk.createSprite(90, 24);
  clk.fillSprite(0x0000);
  clk.setTextSize(2);
  clk.setTextColor(TFT_ORANGE);
  clk.setTextDatum(CC_DATUM);
  clk.drawString(hostInfo.netDownloadData, 45, 12);
  clk.pushSprite(47, 34);

  clk.deleteSprite();
};

void McHost::updateInfo(String str) {
  /* 解析数据 */
  DeserializationError error = deserializeJson(doc, str);
  //检查反序列化是否成功
  if (!error) {
    hostInfo.cpuData = doc["cpuData"];
    hostInfo.memData = doc["memData"];
    const char *netUploadData = doc["downloadData"];
    hostInfo.netUploadData = netUploadData;
    const char *netDownloadData = doc["uploadData"];
    hostInfo.netDownloadData = netDownloadData;
  } else {
    hostInfo.cpuData = hostInfo.memData = 0;
    hostInfo.netUploadData = hostInfo.netDownloadData = "";
  }
};