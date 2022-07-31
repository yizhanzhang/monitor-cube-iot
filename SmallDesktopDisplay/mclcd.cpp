#include "mclcd.h"

#define LCD_BL_PIN 5

extern int BL_addr;
extern int Ro_addr;
extern int LCD_Rotation;
extern int LCD_BL_PWM;
extern uint16_t bgColor;

void McLcd::initLcd(TFT_eSPI *tft)
{
  Serial.println("McLcd: initLcd");
  // 背光亮度设置
  if (EEPROM.read(BL_addr) > 0 && EEPROM.read(BL_addr) < 100)
    LCD_BL_PWM = EEPROM.read(BL_addr);
  // 屏幕方向设置
  if (EEPROM.read(Ro_addr) >= 0 && EEPROM.read(Ro_addr) <= 3)
    LCD_Rotation = EEPROM.read(Ro_addr);

  pinMode(LCD_BL_PIN, OUTPUT);
  analogWrite(LCD_BL_PIN, 1023 - (LCD_BL_PWM * 10));

  tft -> begin(); /* TFT init */
  tft -> invertDisplay(1);//反转所有显示颜色：1反转，0正常
  tft -> setRotation(LCD_Rotation);
  tft -> fillScreen(0x0000);
  tft -> setTextColor(TFT_BLACK, bgColor);
}
