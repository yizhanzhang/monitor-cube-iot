#ifndef MC_WIFI_H
#define MC_WIFI_H

#include <Arduino.h>
#include <Print.h>
#include <SPI.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <TFT_eSPI.h>
#include <WiFiManager.h>
#include <string.h>

#include "../mclcd/mclcd.h"

struct WifiConfigType
{
  char stassid[32]; //  定义配网得到的WIFI名长度(最大32字节)
  char stapsw[64];  //  定义配网得到的WIFI密码长度(最大64字节)
  int  lcdBl;       // 背光强度设定
  char hostIp[15];  // 主机IP地址
};

class McWifi {
  public:
    McWifi();
    WifiConfigType wifiConf;
    void setWifiConfig(const char *stassid, const char *stapsw);
    void readWifiConfig();
    void saveWifiConfig();
    void deleteWifiConfig();
    void link();
    void showWifiAPTip();
    void openWifiAP();
  private:
};

#endif