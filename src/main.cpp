#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <TFT_eSPI.h>
#include <OneButton.h>
#include "global.define.h"
#include "modules/mclcd/mclcd.h"
#include "modules/mcloading/mcloading.h"
#include "modules/mcserial/mcserial.h"
#include "modules/mchost/mchost.h"
#include "modules/mctheme/mcwallpaper/mcwallpaper.h"

/* 全局唯一实例 */
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite clk = TFT_eSprite(&tft);
OneButton touchBtn = OneButton(4, true, false);

McLcd mcLcd;
McLoading mcLoading;
McSerial mcSerial;
McHost mcHost;
McWallpaper mcWallpaper;

int themeCounter;
void changeTheme() {
  themeCounter = (themeCounter + 1) % 2;
  if (themeCounter == 0) {
    mcHost.init();
  } else {
    mcWallpaper.init();
  }
}

void setup()
{
  // 打开调试串口以及EEPROM
  Serial.begin(115200);
  EEPROM.begin(1024);

  // 初始化LCD屏相关配置
  mcLcd.init();

  // theme switch setting
  themeCounter = 0;
  touchBtn.attachDoubleClick(changeTheme);

  // init theme
  mcHost.init();
}



void loop()
{
  mcSerial.serialLoop();
  touchBtn.tick();
  if (themeCounter == 0) {
    mcHost.update();
  } else {
    mcWallpaper.update();
  }
}