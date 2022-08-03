#include "mcwifi.h"

extern int LCD_BL_PWM;
extern int wifi_addr;
extern int BL_addr;
extern TFT_eSprite clk;
extern McLcd mcLcd;
extern WiFiManager wm;

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

  LCD_BL_PWM = getParam("LCDBL").toInt();
  String HOSTIP = getParam("HOSTIP");

  if (EEPROM.read(BL_addr) != LCD_BL_PWM)
  {
    EEPROM.write(BL_addr, LCD_BL_PWM);
    EEPROM.commit();
    delay(5);
    mcLcd.setBrightness(LCD_BL_PWM);
  }
}

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

void McWifi::showWifiAPTip() {
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
};

void McWifi::openWifiAP() {
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