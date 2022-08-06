#ifndef MC_LOADING_H
#define MC_LOADING_H

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>

#include "../../img/loading/ii0.h"
#include "../../img/loading/ii1.h"
#include "../../img/loading/ii2.h"
#include "../../img/loading/ii3.h"
#include "../../img/loading/ii4.h"
#include "../../img/loading/ii5.h"
#include "../../img/loading/ii6.h"
#include "../../img/loading/ii7.h"
#include "../../img/loading/ii8.h"
#include "../../img/loading/ii9.h"
#include "../../img/loading/ii10.h"
#include "../../img/loading/ii11.h"
#include "../../img/loading/ii12.h"
#include "../../img/loading/ii13.h"
#include "../../img/loading/ii14.h"
#include "../../img/loading/ii15.h"
#include "../../img/loading/ii16.h"
#include "../../img/loading/ii17.h"
#include "../../img/loading/ii18.h"
#include "../../img/loading/ii19.h"
#include "../../img/loading/ii20.h"
#include "../../img/loading/ii21.h"
#include "../../img/loading/ii22.h"
#include "../../img/loading/ii23.h"
#include "../../img/loading/ii24.h"
#include "../../img/loading/ii25.h"
#include "../../img/loading/ii26.h"
#include "../../img/loading/ii27.h"
#include "../../img/loading/ii28.h"
#include "../../img/loading/ii29.h"
#include "../../img/loading/ii30.h"
#include "../../img/loading/ii31.h"
#include "../../img/loading/ii32.h"
#include "../../img/loading/ii33.h"
#include "../../img/loading/ii34.h"
#include "../../img/loading/ii35.h"

#define ANIM_DELTA 30
#define ANIM_LOOP_MIN 200
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