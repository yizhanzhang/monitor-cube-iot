#ifndef MC_LCD_H
#define MC_LCD_H

#include <TFT_eSPI.h>
#include <EEPROM.h>

#define LCD_BL_PIN 5 //  LCD背光引脚

class McLcd
{
private:

public:
  void initLcd(TFT_eSPI *tft);
  void setBrightness(TFT_eSPI *tft, int pwm)
};


#endif
