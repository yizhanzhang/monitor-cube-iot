#ifndef MC_LCD_H
#define MC_LCD_H

#include <TFT_eSPI.h>
#include <EEPROM.h>

class McLcd
{
private:

public:
  void initLcd(TFT_eSPI* tft);
};


#endif
