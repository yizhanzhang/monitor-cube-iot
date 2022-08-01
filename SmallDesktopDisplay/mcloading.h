#ifndef MC_LOADING_H
#define MC_LOADING_H

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>

#include "img/picture/donghua/ii0.h"
#include "img/picture/donghua/ii1.h"
#include "img/picture/donghua/ii2.h"
#include "img/picture/donghua/ii3.h"
#include "img/picture/donghua/ii4.h"
#include "img/picture/donghua/ii5.h"
#include "img/picture/donghua/ii6.h"
#include "img/picture/donghua/ii7.h"
#include "img/picture/donghua/ii8.h"
#include "img/picture/donghua/ii9.h"
#include "img/picture/donghua/ii10.h"
#include "img/picture/donghua/ii11.h"
#include "img/picture/donghua/ii12.h"
#include "img/picture/donghua/ii13.h"
#include "img/picture/donghua/ii14.h"
#include "img/picture/donghua/ii15.h"
#include "img/picture/donghua/ii16.h"
#include "img/picture/donghua/ii17.h"
#include "img/picture/donghua/ii18.h"
#include "img/picture/donghua/ii19.h"
#include "img/picture/donghua/ii20.h"
#include "img/picture/donghua/ii21.h"
#include "img/picture/donghua/ii22.h"
#include "img/picture/donghua/ii23.h"
#include "img/picture/donghua/ii24.h"
#include "img/picture/donghua/ii25.h"
#include "img/picture/donghua/ii26.h"
#include "img/picture/donghua/ii27.h"
#include "img/picture/donghua/ii28.h"
#include "img/picture/donghua/ii29.h"
#include "img/picture/donghua/ii30.h"
#include "img/picture/donghua/ii31.h"
#include "img/picture/donghua/ii32.h"
#include "img/picture/donghua/ii33.h"
#include "img/picture/donghua/ii34.h"
#include "img/picture/donghua/ii35.h"

#define ANIM_DELTA 30
#define ANIM_LOOP_MIN 100
#define IMG_X 20
#define IMG_Y 35
#define DESC "powered by yizhanzhang"

class McLoading {
  private:
    int animCounter;
    int animLastTimeStamp;
    int animLoopCounter;
  public:
    McLoading();
    void draw();
    bool loading();
};

#endif