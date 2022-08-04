#include "mcwifi.h"

extern int wifi_addr;
extern int BL_addr;
extern int host_addr;

extern int LCD_BL_PWM;
extern String HOSTIP;

extern TFT_eSprite clk;
extern McLcd mcLcd;
extern WiFiManager wm;
extern McWifi mcWifi;

String getParam(String name);
void saveParamCallback();

McWifi::McWifi(void) {
  wifiConf = WifiConfigType{"", "", 0, ""};
  readWifiConfig();
};

// 从EEPROM读取WiFi配置信息
void McWifi::readWifiConfig() {
  uint8_t *p = (uint8_t*)(&wifiConf);
  for (int i = 0; i < sizeof(wifiConf); i++)
  {
    *(p + i) = EEPROM.read(wifi_addr + i);
  }
  Serial.printf("Read WiFi Config.....\r\n");
  Serial.printf("SSID   :%s\r\n", wifiConf.stassid);
  Serial.printf("PSW    :%s\r\n", wifiConf.stapsw);
  Serial.printf("LCDBL  :%s\r\n", wifiConf.lcdBl);
  Serial.printf("HOSTPI :%s\r\n", wifiConf.hostIp);
};

// 将WIFI配置保存到EEPROM
void McWifi::writeWifiConfig()
{
  strcpy(wifiConf.stassid, WiFi.SSID().c_str());
  strcpy(wifiConf.stapsw, WiFi.psk().c_str());

  writeConfig2EEPROM();
};

// 清空WIFI配置并写入EEPROM
void McWifi::clearWifiConfig()
{
  wifiConf = WifiConfigType{"", "", 0, ""};
  writeConfig2EEPROM();
};

void McWifi::writeConfig2EEPROM() {
  uint8_t *p = (uint8_t*)(&wifiConf);
  for (int i = 0; i < sizeof(wifiConf); i++)
  {
    EEPROM.write(wifi_addr + i, *(p + i)); //在闪存内模拟写入
  }

  delay(10);
  EEPROM.commit();
  delay(10);
}

void McWifi::link() {
  Serial.print("正在连接WIFI ");
  Serial.println(wifiConf.stassid);
  WiFi.begin(wifiConf.stassid, wifiConf.stapsw);
};

void McWifi::openWifiAP() {
  /* draw AP Tip */
  clk.setColorDepth(8);
  clk.createSprite(200, 90);//创建窗口
  clk.fillSprite(0x0000);   //填充率

  clk.setTextDatum(CC_DATUM);   //设置文本数据
  clk.setTextColor(TFT_GREEN, 0x0000);
  clk.drawString("WiFi Connect Fail!", 100, 30, 2);
  clk.drawString("SSID:", 45, 60, 2);
  clk.setTextColor(TFT_YELLOW, 0x0000);
  clk.drawString("MonitorCubeAP", 125, 60, 2);
  clk.pushSprite(20, 37); //窗口位置

  clk.deleteSprite();

  /* open */
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

  delay(3000);
  wm.resetSettings();

  // set dark theme
  wm.setClass("invert");
  // set min RSSI (percentage) to show in scans, null = 8%
  wm.setMinimumSignalQuality(20);

  /* 自定义展示数据 */
  WiFiManagerParameter  custom_bl("LCDBL", "背光强度(1-100)", "8", 3);
  WiFiManagerParameter  custom_host("HOSTIP", "主机IP(IPV4)", "", 15);
  WiFiManagerParameter  p_lineBreak("<p></p>");
  wm.addParameter(&p_lineBreak);
  wm.addParameter(&custom_bl);
  wm.addParameter(&p_lineBreak);
  wm.addParameter(&custom_host);
  wm.setSaveParamsCallback(saveParamCallback);

  // 选择展示的menu
  std::vector<const char *> menu = {"wifi", "restart"};
  wm.setMenu(menu);

  bool res;
  res = wm.autoConnect("MonitorCubeAP"); // anonymous ap

  while (!res);
}

String getParam(String name) {
  String value;
  if (wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback() {
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println("PARAM LCD BackLight = " + getParam("LCDBL"));
  Serial.println("PARAM 主机IP = " + getParam("HOSTIP"));

  mcWifi.wifiConf.lcdBl = getParam("LCDBL").toInt();
  strcpy(mcWifi.wifiConf.hostIp, getParam("HOSTIP").c_str());

  mcLcd.setBrightness(mcWifi.wifiConf.lcdBl);
}
