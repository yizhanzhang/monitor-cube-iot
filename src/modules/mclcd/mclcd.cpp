#include "mclcd.h"

extern TFT_eSPI tft;

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  if ( y >= tft.height() ) return 0;
  tft.pushImage(x, y, w, h, bitmap);
  return 1;
};

void McLcd::init() {
  Serial.println("McLcd: init");
  initLcd();
  initTJpgDec();
  setBrightness(8);
};

void McLcd::initLcd() {
  pinMode(LCD_BL_PIN, OUTPUT);

  tft.init(); /* TFT init */
  tft.invertDisplay(1); //反转所有显示颜色：1反转，0正常
  tft.setRotation(0); // 设定屏幕方向
  tft.fillScreen(TFT_BLACK);

  setBrightness(8);
};

void McLcd::initTJpgDec() {
  TJpgDec.setJpgScale(1);
  TJpgDec.setSwapBytes(true);
  TJpgDec.setCallback(tft_output);
};

void McLcd::setBrightness(int pwm) {
  if (pwm <= 0 || pwm >= 100) {
    pwm = 8;
  }
  analogWrite(LCD_BL_PIN, (1023 - (pwm * 10)) / 4);
  Serial.print("亮度调整为：");
  Serial.println(pwm);
};
