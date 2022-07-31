#include "mcwifi.h"

extern int wifi_addr;

McWifi::McWifi(void) {
  wifiConf = WifiConfigType{"", ""};
};

void McWifi::setWifiConfig(const char *stassid, const char *stapsw) {
  strcpy(wifiConf.stassid, stassid); //名称复制
  strcpy(wifiConf.stapsw, stapsw); //密码复制
}

//  从eeprom读取WiFi信息ssid，psw
void McWifi::readWifiConfig() {
  uint8_t *p = (uint8_t*)(&wifiConf);
  for (int i = 0; i < sizeof(wifiConf); i++)
  {
    *(p + i) = EEPROM.read(i + wifi_addr);
  }
  Serial.printf("Read WiFi Config.....\r\n");
  Serial.printf("SSID:%s\r\n", wifiConf.stassid);
  Serial.printf("PSW:%s\r\n", wifiConf.stapsw);
};

//  wifi ssid，psw保存到eeprom
void McWifi::saveWifiConfig()
{
  uint8_t *p = (uint8_t*)(&wifiConf);
  for (int i = 0; i < sizeof(wifiConf); i++)
  {
    EEPROM.write(i + wifi_addr, *(p + i)); //在闪存内模拟写入
  }
  delay(10);
  EEPROM.commit();//执行写入ROM
  delay(10);
};

void McWifi::deleteWifiConfig()
{
  wifiConf = WifiConfigType{"", ""};
  uint8_t *p = (uint8_t*)(&wifiConf);
  for (int i = 0; i < sizeof(wifiConf); i++)
  {
    EEPROM.write(i + wifi_addr, *(p + i)); //在闪存内模拟写入
  }
  delay(10);
  EEPROM.commit();//执行写入ROM
  delay(10);
};

void McWifi::link() {
  Serial.print("正在连接WIFI ");
  Serial.println(wifiConf.stassid);
  WiFi.begin(wifiConf.stassid, wifiConf.stapsw);
};