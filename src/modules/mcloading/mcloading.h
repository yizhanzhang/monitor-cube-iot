#ifndef MC_LOADING_H
#define MC_LOADING_H

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>

#include "../../img/loading/ii0.h"
#include "../../img/loading/ii2.h"
#include "../../img/loading/ii4.h"
#include "../../img/loading/ii6.h"
#include "../../img/loading/ii8.h"
#include "../../img/loading/ii10.h"
#include "../../img/loading/ii12.h"
#include "../../img/loading/ii14.h"
#include "../../img/loading/ii16.h"
#include "../../img/loading/ii18.h"
#include "../../img/loading/ii20.h"
#include "../../img/loading/ii22.h"
#include "../../img/loading/ii24.h"
#include "../../img/loading/ii26.h"
#include "../../img/loading/ii28.h"
#include "../../img/loading/ii30.h"
#include "../../img/loading/ii32.h"
#include "../../img/loading/ii34.h"

#define ANIM_DELTA 60
#define ANIM_LOOP_MIN 150
#define IMG_X 20
#define IMG_Y 35
#define DESC "powered by yizhanzhang"

class McLoading {
  private:
    int animLastTimeStamp;
    int animLoopCounter;
    void drawText();
    void drawAnim();
  public:
    McLoading();
    void loading();
};

#endif