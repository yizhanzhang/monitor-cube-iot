#include <ArduinoJson.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <TJpg_Decoder.h>
#include <EEPROM.h>
#include <WiFiManager.h>
#include "font/ZdyLwFont_20.h"
#include "src/number/number.h"
#include "src/mclcd/mclcd.h"
#include "src/mcwifi/mcwifi.h"
#include "src/mcloading/mcloading.h"
#include "src/mchost/mchost.h"

/*** Component objects ***/
WiFiManager wm;
Number      dig;
McLcd       mcLcd;
McWifi      mcWifi;
McLoading   mcLoading;
McHost      mcHost;

//LCD屏幕相关设置
TFT_eSPI tft = TFT_eSPI(240, 240);
TFT_eSprite clk = TFT_eSprite(&tft);

uint16_t bgColor = 0x0000;

//EEPROM参数存储地址位
int wifi_addr = 30; //被写入数据的EEPROM地址编号  20wifi-ssid-psw

String SMOD = "";
//串口调试设置函数
void Serial_set() {
  String incomingByte = "";
  if (Serial.available() > 0) {
    while (Serial.available() > 0) //监测串口缓存，当有数据输入时，循环赋值给incomingByte
    {
      incomingByte += char(Serial.read());//读取单个字符值，转换为字符，并按顺序一个个赋值给incomingByte
      delay(2);//不能省略，因为读取缓冲区数据需要时间
    }

    SMOD = incomingByte;
    if (SMOD == "0x05") {
      Serial.println("重置WiFi设置中......");
      wm.resetSettings();
      mcWifi.clearWifiConfig();
      delay(10);
      Serial.println("重置WiFi成功");
      SMOD = "";
      ESP.restart();
    } else {
      Serial.println("");
      Serial.println("请输入需要修改的代码：");
      Serial.println("亮度设置输入        0x01");
      Serial.println("地址设置输入        0x02");
      Serial.println("屏幕方向设置输入    0x03");
      Serial.println("更改天气更新时间    0x04");
      Serial.println("重置WiFi(会重启)    0x05");
      Serial.println("");
    }
  }
}


void setup()
{
  // 打开调试串口以及EEPROM
  Serial.begin(115200);
  EEPROM.begin(1024);

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

  mcHost.init();
}



void loop()
{
  mcHost.update();
  // Serial_set();
}