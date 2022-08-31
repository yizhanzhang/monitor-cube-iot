#include "mcserial.h"

extern McLcd mcLcd;
extern McHost mcHost;

void serialLog(String txt) {
  Serial.println("[MC_SERIAL] " + txt);
};

McSerial::McSerial() {
  initCommandMap();
};

void McSerial::initCommandMap() {
};

//串口调试设置函数
void McSerial::tick() {
  if (!Serial.available()) return void();

  // get inputByte
  String inputByte = "";
  while (Serial.available() > 0) {
    inputByte += char(Serial.read());
    delay(2);
  }
  inputByte.trim();
  serialLog("INCOMING BYTE:" + inputByte);

  // use command
  mcHost.updateInfo(inputByte);
};