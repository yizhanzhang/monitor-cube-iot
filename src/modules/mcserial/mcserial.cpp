#include "mcserial.h"

extern WiFiManager wm;
extern McLcd mcLcd;
extern McWifi mcWifi;

String SMOD = "";
String incomingByte = "";
//串口调试设置函数
void serialSet() {
  String incomingByte = "";
  if (Serial.available() > 0) {
    while (Serial.available() > 0)
    {
      incomingByte += char(Serial.read());
      delay(2);
    }
    if (SMOD == "0x01") {
      Serial.println("设置亮度中......");
      incomingByte.trim();
      int brightness = incomingByte.toInt();
      mcLcd.setBrightness(brightness);
      Serial.print("设置亮度完成:");
      Serial.println(brightness);
      SMOD = "";
    } else if (SMOD == "0x05") {
      Serial.println("重置WiFi设置中......");
      mcWifi.clearWifiConfig();
      wm.resetSettings();
      delay(10);
      Serial.println("重置WiFi成功");
      SMOD = "";
    } else {
      SMOD = incomingByte;
      SMOD.trim();
    }
  }
}