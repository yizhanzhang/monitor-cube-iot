#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <TFT_eSPI.h>
#include <OneButton.h>
#include "global.define.h"
#include "modules/mclcd/mclcd.h"
#include "modules/mcserial/mcserial.h"
#include "modules/mctheme/mchost/mchost.h"
#include "modules/mctheme/mcwallpaper/mcwallpaper.h"
#include "modules/mctheme/manager.h"

/* 全局唯一实例 */
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite clk = TFT_eSprite(&tft);
OneButton touchBtn = OneButton(4, true, false);

McLcd mcLcd;
McSerial mcSerial;
McThemeManager mcThemeManager;

void switchTheme() {
  mcThemeManager.switchTheme();
}

void clickTheme() {
  mcThemeManager.clickTheme();
}

void setup()
{
  // 打开调试串口以及EEPROM
  Serial.begin(115200);
  EEPROM.begin(1024);

  // 初始化LCD屏相关配置
  mcLcd.init();

  // load initial theme
  mcThemeManager.initTheme();

  // theme switch setting
  touchBtn.attachDoubleClick(switchTheme);
  touchBtn.attachClick(clickTheme);
}

void loop()
{
  mcSerial.tick();
  mcThemeManager.tick();
  touchBtn.tick();
}