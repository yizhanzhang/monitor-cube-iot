#include "mcserial.h"

extern WiFiManager wm;
extern McLcd mcLcd;
extern McWifi mcWifi;

McSerial::McSerial() {
  SMOD = "";
  initCommandMap();
};

void McSerial::initCommandMap() {
  SerialCommand lightCommand{"0x01", "调整亮度", "请输入1-100之间的数字"};
  serialCommandMap[lightCommand.key] = lightCommand;

  SerialCommand resetCommand{"0x05", "重置设备", ""};
  serialCommandMap[resetCommand.key] = resetCommand;
};

void McSerial::serialLog(String txt) {
  Serial.println("[MC_SERIAL] " + txt);
};

//串口调试设置函数
void McSerial::serialLoop() {
  if (!Serial.available()) return void();

  // get inputByte
  String inputByte = "";
  while (Serial.available() > 0) {
    inputByte += char(Serial.read());
    delay(2);
  }
  inputByte.trim();
  serialLog("INCOMING BYTE:" + inputByte);

  // check SMOD is existed
  if (SMOD.length()) {
    if (SMOD == "0x01") {
      int brightness = inputByte.toInt();
      mcLcd.setBrightness(brightness);
      serialLog("设置亮度完成:" + brightness);
    } else if (SMOD == "0x05") {
      mcWifi.clearWifiConfig();
      wm.resetSettings();
      serialLog("重置WiFi成功,准备重启");
      delay(1000);
      ESP.restart();
    }
    SMOD = "";
    return void();
  }

  // check inputByte is in serialCommandMap
  if (serialCommandMap.count(inputByte)) {
    SMOD = inputByte;
    SerialCommand targetCmd = serialCommandMap[SMOD];
    serialLog("SET MODE: " + targetCmd.title);
    serialLog(targetCmd.inputTip.length() ? targetCmd.inputTip : "按回车键继续...");
  } else {
    serialLog("可用指令集目录:");
    for (auto iter = serialCommandMap.begin(); iter != serialCommandMap.end(); ++iter) {
      SerialCommand targetCmd = iter->second;
      serialLog("- " + targetCmd.key + ": " + targetCmd.title);
    }
  }
};