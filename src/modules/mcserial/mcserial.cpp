#include "mcserial.h"

extern WiFiManager wm;
extern McLcd mcLcd;
extern McWifi mcWifi;

McSerial::McSerial() {
  SerialCommand lightCommand{"0x01", "调整亮度"};
  mapSerialCommand[lightCommand.id] = lightCommand;
};

//串口调试设置函数
void McSerial::serialLoop() {
  String incomingByte = "";
  if (Serial.available() > 0) {
    while (Serial.available() > 0)
    {
      incomingByte += char(Serial.read());
      delay(2);
    }
    incomingByte.trim();
    Serial.println("[MC_SERIAL] INCOMING BYTE:" + incomingByte);
    if (SMOD == "0x01") {
      Serial.println("设置亮度中......");
      int brightness = incomingByte.toInt();
      mcLcd.setBrightness(brightness);
      Serial.print("设置亮度完成:");
      Serial.println(brightness);
      SMOD = "";
    } else if (SMOD == "0x05") {
      Serial.println("重置WiFi设置中......");
      mcWifi.clearWifiConfig();
      wm.resetSettings();
      Serial.println("重置WiFi成功");
      SMOD = "";
      delay(1000);
      ESP.restart();
    } else {
      SMOD = incomingByte;
      Serial.println("[MC_SERIAL] SET MODE:" + SMOD);
    }
  }
}