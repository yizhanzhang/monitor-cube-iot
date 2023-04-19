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

struct HostInfo {
  int timestamp;
  int cpuData;
  int memData;
  String netUploadData;
  String netDownloadData;
  int stockStatus;
  String stockName;
  String stockData;
};

class McHost: public McThemeBase {
  private:
    HostInfo hostInfo;
    bool isInvalid;
    int timestamp;
    void drawInfo();
  public:
    McHost();
    void init();
    void update();
    void updateInfo(String str);
};

# endif