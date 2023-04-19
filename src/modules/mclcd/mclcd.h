#ifndef MC_LCD_H
#define MC_LCD_H

#include <TFT_eSPI.h>
#include <EEPROM.h>
#include <TJpg_Decoder.h>

#define LCD_BL_PIN 5 //  LCD背光引脚

class McLcd {
  private:
    void initLcd();
    void initTJpgDec();
  public:
    void init();
    void setBrightness(int pwm);
};


#endif
