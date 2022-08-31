#ifndef MC_THEME_BASE
#define MC_THEME_BASE

#include <Arduino.h>

class McThemeBase {
  public:
    virtual void init() {
      Serial.println("McThemeBase init");
    };
    virtual void update() {
      Serial.println("McThemeBase update");
    };
    virtual void click() {
      Serial.println("McThemeBase click");
    };
};

#endif