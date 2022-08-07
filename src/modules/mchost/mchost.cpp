#include "mchost.h"

extern TFT_eSprite clk;
extern TFT_eSPI tft;
extern McWifi mcWifi;

WiFiClient wificlient;
StaticJsonDocument<200> doc;

const int columnOuterWidth = 30;
const int columnOuterHeight = 180;
const int columnOuterRadius =  10;
const int columnOuterThickness = 3;
const int columnInnerWidth = 18;
const int columnInnerHeight = 168;
const int columnInnerRadius =  4;

const uint8_t *astImgArr[] = {
  ast0
};

const uint32_t astImgSizeArr[] = {
  sizeof(ast0)
};

void drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color, int32_t thickness) {
  for (int i = 0; i < thickness; i++) {
    clk.drawRoundRect(x + i, y + i, w - 2 * i, h - 2 * i, r - 2 * i, color);
  }
};

McHost::McHost(void) {
  hostInfo = HostInfo{ 0, 0, "00.00M", "00.00M" };
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
  counterAst = (counterAst + 1) % 6;
};

void McHost::fetchInfo() {
  /* fetch info */
  String URL = mcWifi.wifiConf.hostIp;
  if (!hostIp.startsWith("http://") && !hostIp.startsWith("https://")) {
    URL = "http://" + URL
  }
  //创建 HTTPClient 对象
  HTTPClient httpClient;
  httpClient.begin(wificlient, URL);
  //启动连接并发送HTTP请求
  int httpCode = httpClient.GET();
  Serial.print("Send GET request to Info URL: ");
  Serial.println(URL);
  //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
  String str = "";
  if (httpCode == HTTP_CODE_OK) {
    str = httpClient.getString();
    Serial.println("获取主机信息成功");
    Serial.println(str);
  } else {
    Serial.println("请求主机信息失败");
    Serial.println(httpCode);
  }
  //关闭ESP8266与服务器连接
  httpClient.end();

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
  // write not download
  clk.setColorDepth(8);
  clk.createSprite(90, 24);
  clk.fillSprite(0x0000);
  clk.setTextSize(2);
  clk.setTextColor(TFT_ORANGE);
  clk.setTextDatum(CC_DATUM);
  clk.drawString(hostInfo.netDownloadData, 45, 12);
  clk.pushSprite(47, 34);

  clk.deleteSprite();
  // write net download
};

void McHost::updateInfo() {
  fetchInfo();
  drawInfo();
}


void McHost::update() {
  int nowStamp = millis();

  if (nowStamp - timestampInfo > TIME_GAP_INFO) {
    updateInfo();
    timestampInfo = millis();
  } else if (nowStamp < timestampInfo) { // 兼容milles极限
    timestampInfo = millis();
  }
};