#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <TFT_eSPI.h>
#include <WiFiManager.h>
#include <OneButton.h>
#include "global.define.h"
#include "modules/mclcd/mclcd.h"
#include "modules/mcwifi/mcwifi.h"
#include "modules/mcloading/mcloading.h"
#include "modules/mchost/mchost.h"
#include "modules/mcwallpaper/mcwallpaper.h"
#include "modules/mcserial/mcserial.h"

/* 全局唯一实例 */
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite clk = TFT_eSprite(&tft);
OneButton touchBtn = OneButton(4, true, false);

WiFiManager wm;
McLcd mcLcd;
McWifi mcWifi;
McLoading mcLoading;
McHost mcHost;
McWallpaper mcWallpaper;

int themeCounter = 0;
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
  if (DEBUG) {
    delay(3000);
    Serial.println("wait for Serial ready in DEBUG env");
  }

  // 从EEPROM中恢复配置
  mcWifi.readWifiConfig();

  // 初始化LCD屏相关配置
  mcLcd.initLcd();
  // 初始化图片库能力
  mcLcd.initTJpgDec();

  // 按照配置尝试进行连接
  mcWifi.link();
  // 显示开机画面,等待WIFI连接
  mcLoading.loading();
  // 如果WIFI连接失败那么打开AP，进行手机配置
  if (WiFi.status() != WL_CONNECTED) {
    mcWifi.openWifiAP();
  }

  // 从WIFI实例中读取wifi配置写入EEPROM
  if (WiFi.status() == WL_CONNECTED)
  {
    mcWifi.writeWifiConfig();
  }

  touchBtn.attachDoubleClick(changeTheme);

  mcHost.init();
}



void loop()
{
  touchBtn.tick();
  if (themeCounter == 0) {
    mcHost.update();
  } else {
    mcWallpaper.draw();
  }
  serialSet();
}