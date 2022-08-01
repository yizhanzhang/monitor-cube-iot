#include "mclcd.h"

extern TFT_eSPI tft;
extern TFT_eSprite clk;

extern int BL_addr;
extern int Ro_addr;
extern int LCD_Rotation;
extern int LCD_BL_PWM;
extern uint16_t bgColor;

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  if ( y >= tft.height() ) return 0;
  tft.pushImage(x, y, w, h, bitmap);
  return 1;
};

void McLcd::initLcd() {
  Serial.println("McLcd: initLcd");

  // 背光亮度设置
  if (EEPROM.read(BL_addr) > 0 && EEPROM.read(BL_addr) < 100)
    LCD_BL_PWM = EEPROM.read(BL_addr);
  // 屏幕方向设置
  if (EEPROM.read(Ro_addr) >= 0 && EEPROM.read(Ro_addr) <= 3)
    LCD_Rotation = EEPROM.read(Ro_addr);

  pinMode(LCD_BL_PIN, OUTPUT);
  analogWrite(LCD_BL_PIN, 1023 - (LCD_BL_PWM * 10));

  tft.begin(); /* TFT init */
  tft.invertDisplay(1);//反转所有显示颜色：1反转，0正常
  tft.setRotation(LCD_Rotation);
  tft.fillScreen(0x0000);
  tft.setTextColor(TFT_BLACK, bgColor);
};

void McLcd::setBrightness(TFT_eSPI *tft, int pwm) {
  Serial.println("McLcd: setBrightness");
  analogWrite(LCD_BL_PIN, 1023 - (pwm * 10));
  Serial.println("亮度调整为：" + LCD_BL_PWM);
};

void McLcd::initTJpgDec() {
  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(tft_output);
};
