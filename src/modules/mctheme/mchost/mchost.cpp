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

void drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color, int32_t thickness) {
  for (int i = 0; i < thickness; i++) {
    clk.drawRoundRect(x + i, y + i, w - 2 * i, h - 2 * i, r - 2 * i, color);
  }
};

McHost::McHost(void) {
  hostInfo = HostInfo{ 0, 0, "00.00M", "00.00M" };
  timestampInfo = 0;
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
  // stock layout
  clk.setColorDepth(8);
  clk.createSprite(110, 30);
  clk.fillSprite(TFT_BLUE);
  clk.setTextSize(2);
  clk.setTextColor(TFT_WHITE);
  clk.setTextDatum(CC_DATUM);
  clk.drawString("TENCENT", 55, 16);
  clk.pushSprite(18, 120);
  clk.deleteSprite();
};

void McHost::update() {
  int nowStamp = millis();

  if (nowStamp - timestampInfo > TIME_GAP_INFO) {
    drawInfo();
    timestampInfo = millis();
  }
  // 兼容milles极限
  if (nowStamp < timestampInfo) { 
    timestampInfo = millis();
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
  clk.deleteSprite();
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
  // write stock
  clk.setColorDepth(8);
  clk.createSprite(115, 40);
  clk.fillSprite(TFT_BLACK);
  clk.setTextSize(4);
  clk.setTextDatum(CC_DATUM);
  if (hostInfo.stockStatus == 0) {
    clk.setTextColor(TFT_LIGHTGREY);
  } else if (hostInfo.stockStatus == 1) {
    clk.setTextColor(TFT_RED);
  } else {
    clk.setTextColor(TFT_GREEN);
  }
  clk.drawString(hostInfo.stockData, 60, 20);
  clk.pushSprite(18, 164);
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
    hostInfo.stockStatus = doc["stockStatus"];
    const char *stockData = doc["stockData"];
    hostInfo.stockData = stockData;
  } else {
    hostInfo.cpuData = hostInfo.memData = hostInfo.stockStatus = 0;
    hostInfo.netUploadData = hostInfo.netDownloadData = hostInfo.stockData = "";
  }
};