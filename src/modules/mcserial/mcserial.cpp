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

  // use command
  String str = "{\"cpuData\": 33, \"memData\": 22, \"downloadData\": \"0.33MB\", \"uploadData\": \"0.22MB\" }";
  mcHost.fetchInfo(str);
};