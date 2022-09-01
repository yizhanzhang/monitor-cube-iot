#ifndef MC_HOST_H
#define MC_HOST_H

#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>
#include <ESP8266HTTPClient.h>

#include "../base.h"
#include "../../../img/host/i0.h"
#include "../../../img/host/i1.h"
#include "../../../img/host/download.h"
#include "../../../img/host/upload.h"
#include "../../../img/astronaut/i0.h"
#include "../../../img/astronaut/i4.h"
#include "../../../img/astronaut/i8.h"
#include "../../../img/astronaut/i12.h"
#include "../../../img/astronaut/i16.h"
#include "../../../img/astronaut/i20.h"
#include "../../../img/astronaut/i24.h"
#include "../../../img/astronaut/i28.h"
#include "../../../img/astronaut/i32.h"
#include "../../../img/astronaut/i36.h"

#define TIME_GAP_INFO 1000
#define TIME_GAP_AST  200

struct HostInfo {
  int cpuData;
  int memData;
  String netUploadData;
  String netDownloadData;
};

class McHost: public McThemeBase {
  private:
    HostInfo hostInfo;
    bool hasInited;
    int timestampInfo;
    int timestampAst;
    int counterAst;
    void drawAst();
    void drawInfo();
  public:
    McHost();
    void init();
    void update();
    void updateInfo(String str);
};

# endif