#include "ArduinoJson.h"
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
#include "weathernum.h"
#include "src/number/number.h"
#include "src/mclcd/mclcd.h"
#include "src/mcwifi/mcwifi.h"
#include "src/mcloading/mcloading.h"

//WiFiManager 参数
WiFiManager wm; // global wm instance


/* *****************************************************************
    字库、图片库
 * *****************************************************************/
#include "font/ZdyLwFont_20.h"
#include "img/temperature.h"
#include "img/humidity.h"

#include "img/picture/tkr/i0.h"
#include "img/picture/tkr/i1.h"
#include "img/picture/tkr/i2.h"
#include "img/picture/tkr/i3.h"
#include "img/picture/tkr/i4.h"
#include "img/picture/tkr/i5.h"
#include "img/picture/tkr/i6.h"
#include "img/picture/tkr/i7.h"
#include "img/picture/tkr/i8.h"
#include "img/picture/tkr/i9.h"

int AnimLoad = 0;           //开机图标显示指针记录
int appearTimeLoad = 0;      //更新时间记录
int Anim = 0;        //太空人图标显示指针记录
int appearTime = 0;      //太空人更新时间记录







/* *****************************************************************
    参数设置
 * *****************************************************************/

//天气更新时间  X 分钟
int updateweater_time = 10;

//----------------------------------------------------

/*** Component objects ***/
Number      dig;
WeatherNum  wrat;
McLcd       mcLcd;
McWifi      mcWifi;
McLoading   mcLoading;

//LCD屏幕相关设置
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite clk = TFT_eSprite(&tft);

uint16_t bgColor = 0x0000;

//其余状态标志位
int LCD_Rotation = 0;   //LCD屏幕方向
int LCD_BL_PWM = 8;//屏幕亮度0-100，默认10
uint8_t Wifi_en = 1; //wifi状态标志位  1：打开    0：关闭
uint8_t UpdateWeater_en = 0; //更新时间标志位
int prevTime = 0;       //滚动显示更新标志位
int DHT_img_flag = 0;   //DHT传感器使用标志位


time_t prevDisplay = 0;       //显示时间显示记录
unsigned long weaterTime = 0; //天气更新时间记录

String cityCode = "0";  //天气城市代码 
String HOSTIP = "";
int tempnum = 0;   //温度百分比
int huminum = 0;   //湿度百分比
int tempcol = 0xffff;  //温度显示颜色
int humicol = 0xffff;  //湿度显示颜色


//EEPROM参数存储地址位
int BL_addr = 1;//被写入数据的EEPROM地址编号  1亮度
int Ro_addr = 2; //被写入数据的EEPROM地址编号  2 旋转方向
int DHT_addr = 3;//3 DHT使能标志位
int CC_addr = 10;//被写入数据的EEPROM地址编号  10城市
int wifi_addr = 30; //被写入数据的EEPROM地址编号  20wifi-ssid-psw
int host_addr = 130; //被写入数据的EEPROM主机IP地址


//NTP服务器参数
static const char ntpServerName[] = "ntp6.aliyun.com";
const int timeZone = 8;     //东八区

//wifi连接UDP设置参数
WiFiUDP Udp;
WiFiClient wificlient;
unsigned int localPort = 8000;
float duty = 0;


//函数声明
time_t getNtpTime();
void digitalClockDisplay(int reflash_en);
void printDigits(int digits);
String num2str(int digits);
void sendNTPpacket(IPAddress &address);
void LCD_reflash(int en);


/* *****************************************************************
    函数
 * *****************************************************************/

//湿度图标显示函数
void humidityWin()
{
  clk.setColorDepth(8);

  huminum = huminum / 2;
  clk.createSprite(52, 6);  //创建窗口
  clk.fillSprite(0x0000);    //填充率
  clk.drawRoundRect(0, 0, 52, 6, 3, 0xFFFF); //空心圆角矩形  起始位x,y,长度，宽度，圆弧半径，颜色
  clk.fillRoundRect(1, 1, huminum, 4, 2, humicol); //实心圆角矩形
  clk.pushSprite(45, 222); //窗口位置
  clk.deleteSprite();
}

//温度图标显示函数
void tempWin()
{
  clk.setColorDepth(8);

  clk.createSprite(52, 6);  //创建窗口
  clk.fillSprite(0x0000);    //填充率
  clk.drawRoundRect(0, 0, 52, 6, 3, 0xFFFF); //空心圆角矩形  起始位x,y,长度，宽度，圆弧半径，颜色
  clk.fillRoundRect(1, 1, tempnum, 4, 2, tempcol); //实心圆角矩形
  clk.pushSprite(45, 192); //窗口位置
  clk.deleteSprite();
}

String SMOD = "";//0亮度
//串口调试设置函数
void Serial_set()
{
  String incomingByte = "";
  if (Serial.available() > 0)
  {

    while (Serial.available() > 0) //监测串口缓存，当有数据输入时，循环赋值给incomingByte
    {
      incomingByte += char(Serial.read());//读取单个字符值，转换为字符，并按顺序一个个赋值给incomingByte
      delay(2);//不能省略，因为读取缓冲区数据需要时间
    }
    if (SMOD == "0x01") //设置1亮度设置
    {
      int LCDBL = atoi(incomingByte.c_str());//int n = atoi(xxx.c_str());//String转int
      if (LCDBL >= 0 && LCDBL <= 100)
      {
        EEPROM.write(BL_addr, LCDBL);//亮度地址写入亮度值
        EEPROM.commit();//保存更改的数据
        delay(5);
        LCD_BL_PWM = EEPROM.read(BL_addr);
        delay(5);
        SMOD = "";
        mcLcd.setBrightness(LCD_BL_PWM);
      }
      else
        Serial.println("亮度调整错误，请输入0-100");
    }
    if (SMOD == "0x02") //设置2地址设置
    {
      int CityCODE = 0;
      int CityC = atoi(incomingByte.c_str());//int n = atoi(xxx.c_str());//String转int
      if (CityC >= 101000000 && CityC <= 102000000 || CityC == 0)
      {
        for (int cnum = 0; cnum < 5; cnum++)
        {
          EEPROM.write(CC_addr + cnum, CityC % 100); //城市地址写入城市代码
          EEPROM.commit();//保存更改的数据
          CityC = CityC / 100;
          delay(5);
        }
        for (int cnum = 5; cnum > 0; cnum--)
        {
          CityCODE = CityCODE * 100;
          CityCODE += EEPROM.read(CC_addr + cnum - 1);
          delay(5);
        }

        cityCode = CityCODE;

        if (cityCode == "0")
        {
          Serial.println("城市代码调整为：自动");
          getCityCode();  //获取城市代码
        }
        else
        {
          Serial.printf("城市代码调整为：");
          Serial.println(cityCode);
        }
        Serial.println("");
        getCityWeater();//更新城市天气
        SMOD = "";
      }
      else
        Serial.println("城市调整错误，请输入9位城市代码，自动获取请输入0");
    }
    if (SMOD == "0x03") //设置3屏幕显示方向
    {
      int RoSet = atoi(incomingByte.c_str());
      if (RoSet >= 0 && RoSet <= 3)
      {
        EEPROM.write(Ro_addr, RoSet);//屏幕方向地址写入方向值
        EEPROM.commit();//保存更改的数据
        SMOD = "";
        //设置屏幕方向后重新刷屏并显示
        tft.setRotation(RoSet);
        tft.fillScreen(0x0000);
        LCD_reflash(1);//屏幕刷新程序
        UpdateWeater_en = 1;
        TJpgDec.drawJpg(15, 183, temperature, sizeof(temperature)); //温度图标
        TJpgDec.drawJpg(15, 213, humidity, sizeof(humidity)); //湿度图标
        // getCityWeater();
        // digitalClockDisplay(1);
        // scrollBanner();


        Serial.print("屏幕方向设置为：");
        Serial.println(RoSet);
      }
      else
      {
        Serial.println("屏幕方向值错误，请输入0-5内的值");
      }
    }
    if (SMOD == "0x04") //设置天气更新时间
    {
      int wtup = atoi(incomingByte.c_str());//int n = atoi(xxx.c_str());//String转int
      if (wtup >= 1 && wtup <= 60)
      {
        updateweater_time = wtup;
        SMOD = "";
        Serial.printf("天气更新时间更改为：");
        Serial.print(updateweater_time);
        Serial.println("分钟");
      }
      else
        Serial.println("更新时间太长，请重新设置（1-60）");
    }
    else
    {
      SMOD = incomingByte;
      delay(2);
      if (SMOD == "0x01")
        Serial.println("请输入亮度值，范围0-100");
      else if (SMOD == "0x02")
        Serial.println("请输入9位城市代码，自动获取请输入0");
      else if (SMOD == "0x03")
      {
        Serial.println("请输入屏幕方向值，");
        Serial.println("0-USB接口朝下");
        Serial.println("1-USB接口朝上");
   //     Serial.println("2-USB接口朝上");
    //    Serial.println("3-USB接口朝左");
        Serial.println("2-USB Mirror up");
        Serial.println("3-USB Mirror up");
      }
      else if (SMOD == "0x04")
      {
        Serial.print("当前天气更新时间：");
        Serial.print(updateweater_time);
        Serial.println("分钟");
        Serial.println("请输入天气更新时间（1-60）分钟");
      }
      else if (SMOD == "0x05")
      {
        Serial.println("重置WiFi设置中......");
        delay(10);
        wm.resetSettings();
        mcWifi.deleteWifiConfig();
        delay(10);
        Serial.println("重置WiFi成功");
        SMOD = "";
        ESP.restart();
      }
      else
      {
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
}


void setup()
{
  // 打开调试串口以及EEPROM
  Serial.begin(115200);
  EEPROM.begin(1024);

  // 初始化LCD屏相关配置
  mcLcd.initLcd();
  // 初始化图片库能力
  mcLcd.initTJpgDec();

  /* 初始化wifi相关配置 */
  // 从EEPROM中读取配置
  mcWifi.readWifiConfig();
  // 按照配置尝试进行连接
  mcWifi.link();

  // 显示开机画面,等待WIFI连接,如果WIFI连接失败那么打开AP，进行手机配置
  // bool isConnected = mcLoading.loading();
  // if (!isConnected) {
    mcWifi.showWifiAPTip();
    mcWifi.openWifiAP();
  // }

  delay(10);

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("SSID:");
    Serial.println(WiFi.SSID().c_str());
    Serial.print("PSW:");
    Serial.println(WiFi.psk().c_str());
    mcWifi.setWifiConfig(WiFi.SSID().c_str(), WiFi.psk().c_str());
    mcWifi.saveWifiConfig();
    mcWifi.readWifiConfig();
  }

  Serial.print("本地IP： ");
  Serial.println(WiFi.localIP());
  Serial.println("启动UDP");
  Udp.begin(localPort);
  Serial.println("等待同步...");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  int CityCODE = 0;
  for (int cnum = 5; cnum > 0; cnum--)
  {
    CityCODE = CityCODE * 100;
    CityCODE += EEPROM.read(CC_addr + cnum - 1);
    delay(5);
  }
  if (CityCODE >= 101000000 && CityCODE <= 102000000)
    cityCode = CityCODE;
  else
    getCityCode();  //获取城市代码

  tft.fillScreen(TFT_BLACK);//清屏

  TJpgDec.drawJpg(15, 183, temperature, sizeof(temperature)); //温度图标
  TJpgDec.drawJpg(15, 213, humidity, sizeof(humidity)); //湿度图标

  getCityWeater();

  WiFi.forceSleepBegin(); //wifi off
  Serial.println("WIFI休眠......");
  Wifi_en = 0;
}



void loop()
{
  LCD_reflash(0);
  Serial_set();
}

void LCD_reflash(int en)
{
  if (now() != prevDisplay || en == 1)
  {
    prevDisplay = now();
    digitalClockDisplay(en);
    prevTime = 0;
  }

  //两秒钟更新一次
  if (second() % 2 == 0 && prevTime == 0 || en == 1) {
    scrollBanner();
  }
  if (DHT_img_flag == 0)
    imgAnim();


  if (millis() - weaterTime > (60000 * updateweater_time) || en == 1 || UpdateWeater_en != 0) { //10分钟更新一次天气
    if (Wifi_en == 0)
    {
      WiFi.forceSleepWake();//wifi on
      Serial.println("WIFI恢复......");
      Wifi_en = 1;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("WIFI已连接");
      getCityWeater();
      if (UpdateWeater_en != 0) UpdateWeater_en = 0;
      weaterTime = millis();
      while (!getNtpTime());
      WiFi.forceSleepBegin(); // Wifi Off
      Serial.println("WIFI休眠......");
      Wifi_en = 0;
    }
  }
}

// 发送HTTP请求并且将服务器响应通过串口输出
void getCityCode() {
  String URL = "http://wgeo.weather.com.cn/ip/?_=" + String(now());
  //创建 HTTPClient 对象
  HTTPClient httpClient;

  //配置请求地址。此处也可以不使用端口号和PATH而单纯的
  httpClient.begin(wificlient, URL);

  //设置请求头中的User-Agent
  httpClient.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");
  httpClient.addHeader("Referer", "http://www.weather.com.cn/");

  //启动连接并发送HTTP请求
  int httpCode = httpClient.GET();
  Serial.print("Send GET request to URL: ");
  Serial.println(URL);

  //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
  if (httpCode == HTTP_CODE_OK) {
    String str = httpClient.getString();

    int aa = str.indexOf("id=");
    if (aa > -1)
    {
      //cityCode = str.substring(aa+4,aa+4+9).toInt();
      cityCode = str.substring(aa + 4, aa + 4 + 9);
      Serial.println(cityCode);
      getCityWeater();
    }
    else
    {
      Serial.println("获取城市代码失败");
    }


  } else {
    Serial.println("请求城市代码错误：");
    Serial.println(httpCode);
  }

  //关闭ESP8266与服务器连接
  httpClient.end();
}



// 获取城市天气
void getCityWeater() {
  //String URL = "http://d1.weather.com.cn/dingzhi/" + cityCode + ".html?_="+String(now());//新
  String URL = "http://d1.weather.com.cn/weather_index/" + cityCode + ".html?_=" + String(now()); //原来
  //创建 HTTPClient 对象
  HTTPClient httpClient;

  httpClient.begin(wificlient, URL);

  //设置请求头中的User-Agent
  httpClient.setUserAgent("Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1");
  httpClient.addHeader("Referer", "http://www.weather.com.cn/");

  //启动连接并发送HTTP请求
  int httpCode = httpClient.GET();
  Serial.println("正在获取天气数据");
  Serial.println(URL);

  //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
  if (httpCode == HTTP_CODE_OK) {

    String str = httpClient.getString();
    int indexStart = str.indexOf("weatherinfo\":");
    int indexEnd = str.indexOf("};var alarmDZ");

    String jsonCityDZ = str.substring(indexStart + 13, indexEnd);
    //Serial.println(jsonCityDZ);

    indexStart = str.indexOf("dataSK =");
    indexEnd = str.indexOf(";var dataZS");
    String jsonDataSK = str.substring(indexStart + 8, indexEnd);
    //Serial.println(jsonDataSK);


    indexStart = str.indexOf("\"f\":[");
    indexEnd = str.indexOf(",{\"fa");
    String jsonFC = str.substring(indexStart + 5, indexEnd);
    //Serial.println(jsonFC);

    weaterData(&jsonCityDZ, &jsonDataSK, &jsonFC);
    Serial.println("获取成功");

  } else {
    Serial.println("请求城市天气错误：");
    Serial.print(httpCode);
  }

  //关闭ESP8266与服务器连接
  httpClient.end();
}


String scrollText[7];
//int scrollTextWidth = 0;
//天气信息写到屏幕上
void weaterData(String *cityDZ, String *dataSK, String *dataFC)
{
  //解析第一段JSON
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, *dataSK);
  JsonObject sk = doc.as<JsonObject>();

  //TFT_eSprite clkb = TFT_eSprite(&tft);

  /***绘制相关文字***/
  clk.setColorDepth(8);
  clk.loadFont(ZdyLwFont_20);

  //温度
  clk.createSprite(58, 24);
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_WHITE, bgColor);
  clk.drawString(sk["temp"].as<String>() + "℃", 28, 13);
  clk.pushSprite(100, 184);
  clk.deleteSprite();
  tempnum = sk["temp"].as<int>();
  tempnum = tempnum + 10;
  if (tempnum < 10)
    tempcol = 0x00FF;
  else if (tempnum < 28)
    tempcol = 0x0AFF;
  else if (tempnum < 34)
    tempcol = 0x0F0F;
  else if (tempnum < 41)
    tempcol = 0xFF0F;
  else if (tempnum < 49)
    tempcol = 0xF00F;
  else
  {
    tempcol = 0xF00F;
    tempnum = 50;
  }
  tempWin();

  //湿度
  clk.createSprite(58, 24);
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_WHITE, bgColor);
  clk.drawString(sk["SD"].as<String>(), 28, 13);
  //clk.drawString("100%",28,13);
  clk.pushSprite(100, 214);
  clk.deleteSprite();
  //String A = sk["SD"].as<String>();
  huminum = atoi((sk["SD"].as<String>()).substring(0, 2).c_str());

  if (huminum > 90)
    humicol = 0x00FF;
  else if (huminum > 70)
    humicol = 0x0AFF;
  else if (huminum > 40)
    humicol = 0x0F0F;
  else if (huminum > 20)
    humicol = 0xFF0F;
  else
    humicol = 0xF00F;
  humidityWin();


  //城市名称
  clk.createSprite(94, 30);
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_WHITE, bgColor);
  clk.drawString(sk["cityname"].as<String>(), 44, 16);
  clk.pushSprite(15, 15);
  clk.deleteSprite();

  //PM2.5空气指数
  uint16_t pm25BgColor = tft.color565(156, 202, 127); //优
  String aqiTxt = "优";
  int pm25V = sk["aqi"];
  if (pm25V > 200) {
    pm25BgColor = tft.color565(136, 11, 32); //重度
    aqiTxt = "重度";
  } else if (pm25V > 150) {
    pm25BgColor = tft.color565(186, 55, 121); //中度
    aqiTxt = "中度";
  } else if (pm25V > 100) {
    pm25BgColor = tft.color565(242, 159, 57); //轻
    aqiTxt = "轻度";
  } else if (pm25V > 50) {
    pm25BgColor = tft.color565(247, 219, 100); //良
    aqiTxt = "良";
  }
  clk.createSprite(56, 24);
  clk.fillSprite(bgColor);
  clk.fillRoundRect(0, 0, 50, 24, 4, pm25BgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(0x0000);
  clk.drawString(aqiTxt, 25, 13);
  clk.pushSprite(104, 18);
  clk.deleteSprite();

  scrollText[0] = "实时天气 " + sk["weather"].as<String>();
  scrollText[1] = "空气质量 " + aqiTxt;
  scrollText[2] = "风向 " + sk["WD"].as<String>() + sk["WS"].as<String>();

  //scrollText[6] = atoi((sk["weathercode"].as<String>()).substring(1,3).c_str()) ;

  //天气图标
  wrat.printfweather(170, 15, atoi((sk["weathercode"].as<String>()).substring(1, 3).c_str()));


  //左上角滚动字幕
  //解析第二段JSON
  deserializeJson(doc, *cityDZ);
  JsonObject dz = doc.as<JsonObject>();
  //Serial.println(sk["ws"].as<String>());
  //横向滚动方式
  //String aa = "今日天气:" + dz["weather"].as<String>() + "，温度:最低" + dz["tempn"].as<String>() + "，最高" + dz["temp"].as<String>() + " 空气质量:" + aqiTxt + "，风向:" + dz["wd"].as<String>() + dz["ws"].as<String>();
  //scrollTextWidth = clk.textWidth(scrollText);
  //Serial.println(aa);
  scrollText[3] = "今日" + dz["weather"].as<String>();

  deserializeJson(doc, *dataFC);
  JsonObject fc = doc.as<JsonObject>();

  scrollText[4] = "最低温度" + fc["fd"].as<String>() + "℃";
  scrollText[5] = "最高温度" + fc["fc"].as<String>() + "℃";

  //Serial.println(scrollText[0]);

  clk.unloadFont();
}

int currentIndex = 0;
TFT_eSprite clkb = TFT_eSprite(&tft);

void scrollBanner() {
  //if(millis() - prevTime > 2333) //3秒切换一次
  //  if(second()%2 ==0&& prevTime == 0)
  //  {
  if (scrollText[currentIndex])
  {
    clkb.setColorDepth(8);
    clkb.loadFont(ZdyLwFont_20);
    clkb.createSprite(150, 30);
    clkb.fillSprite(bgColor);
    clkb.setTextWrap(false);
    clkb.setTextDatum(CC_DATUM);
    clkb.setTextColor(TFT_SILVER, bgColor);
    clkb.drawString(scrollText[currentIndex], 74, 16);
    clkb.pushSprite(10, 45);

    clkb.deleteSprite();
    clkb.unloadFont();

    if (currentIndex >= 5)
      currentIndex = 0;  //回第一个
    else
      currentIndex += 1;  //准备切换到下一个
  }
  prevTime = 1;
  //  }
}



void imgAnim()
{
  int x = 160, y = 160;                               //int Anim = 0;      太空人图标显示指针记录
  if (millis() - appearTime > 50) //x ms切换一次         // int appearTime = 0; 太空人更新时间记录

  {
    Anim++;
    appearTime = millis();
  }
  if (Anim == 10)
    Anim = 0;

  switch (Anim)
  {
    case 0:
      TJpgDec.drawJpg(x, y, i0, sizeof(i0));
      break;
    case 1:
      TJpgDec.drawJpg(x, y, i1, sizeof(i1));
      break;
    case 2:
      TJpgDec.drawJpg(x, y, i2, sizeof(i2));
      break;
    case 3:
      TJpgDec.drawJpg(x, y, i3, sizeof(i3));
      break;
    case 4:
      TJpgDec.drawJpg(x, y, i4, sizeof(i4));
      break;
    case 5:
      TJpgDec.drawJpg(x, y, i5, sizeof(i5));
      break;
    case 6:
      TJpgDec.drawJpg(x, y, i6, sizeof(i6));
      break;
    case 7:
      TJpgDec.drawJpg(x, y, i7, sizeof(i7));
      break;
    case 8:
      TJpgDec.drawJpg(x, y, i8, sizeof(i8));
      break;
    case 9:
      TJpgDec.drawJpg(x, y, i9, sizeof(i9));
      break;
    default:
      Serial.println("显示Anim错误");
      break;
  }
}


unsigned char Hour_sign   = 60;
unsigned char Minute_sign = 60;
unsigned char Second_sign = 60;
void digitalClockDisplay(int reflash_en)
{
  int timey = 82;
  if (hour() != Hour_sign || reflash_en == 1) //时钟刷新
  {
    dig.printfW3660(20, timey, hour() / 10);
    dig.printfW3660(60, timey, hour() % 10);
    Hour_sign = hour();
  }
  if (minute() != Minute_sign  || reflash_en == 1) //分钟刷新
  {
    dig.printfO3660(101, timey, minute() / 10);
    dig.printfO3660(141, timey, minute() % 10);
    Minute_sign = minute();
  }
  if (second() != Second_sign  || reflash_en == 1) //分钟刷新
  {
    dig.printfW1830(182, timey + 30, second() / 10);
    dig.printfW1830(202, timey + 30, second() % 10);
    Second_sign = second();
  }

  if (reflash_en == 1) reflash_en = 0;
  /***日期****/
  clk.setColorDepth(8);
  clk.loadFont(ZdyLwFont_20);

  //星期
  clk.createSprite(58, 30);
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_SILVER, bgColor);
  clk.drawString(week(), 29, 16);
  clk.pushSprite(102, 150);
  clk.deleteSprite();

  //月日
  clk.createSprite(95, 30);
  clk.fillSprite(bgColor);
  clk.setTextDatum(CC_DATUM);
  clk.setTextColor(TFT_SILVER, bgColor);
  clk.drawString(monthDay(), 49, 16);
  clk.pushSprite(5, 150);
  clk.deleteSprite();

  clk.unloadFont();
  /***日期****/
}

//星期
String week()
{
  String wk[7] = {"日", "一", "二", "三", "四", "五", "六"};
  String s = "周" + wk[weekday() - 1];
  return s;
}

//月日
String monthDay()
{
  String s = String(month());
  s = s + "月" + day() + "日";
  return s;
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP时间在消息的前48字节中
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  //Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  //Serial.print(ntpServerName);
  //Serial.print(": ");
  //Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      //Serial.println(secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR);
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // 无法获取时间时返回0
}

// 向NTP服务器发送请求
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
