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
#include "../../../img/host/error.h"
#include "../../../img/host/unlink.h"
#include "../../../font/WeChatSansStd_Regular10pt7b.h"
#include "../../../font/WeChatSansStd_Regular20pt7b.h"

#define MYFONT10 &WeChatSansStd_Regular10pt7b
#define MYFONT20 &WeChatSansStd_Regular20pt7b

#define RENDER_TIME_GAP 1000
#define INFO_VALID_GAP 10000

#define POS_ERROR_X 60
#define POS_ERROR_Y 60

#define COLUMN_OUTER_WIDTH 30
#define COLUMN_OUTER_HEIGHT 180
#define COLUMN_OUTER_RADIUS 10
#define COLUMN_OUTER_THICKNESS 3
#define COLUMN_INNER_WIDTH 18
#define COLUMN_INNER_HEIGHT 168
#define COLUMN_INNER_RADIUS 4

#define POS_CPU_COLUMN_X 140
#define POS_CPU_COLUMN_Y 20
#define POS_CPU_ICON_X 145
#define POS_CPU_ICON_Y 210
#define POS_MEM_COLUMN_X 190
#define POS_MEM_COLUMN_Y 20
#define POS_MEM_ICON_X 195
#define POS_MEM_ICON_Y 210

#define POS_NET_UPLOAD_ICON_X 20
#define POS_NET_UPLOAD_ICON_Y 35
#define POS_NET_DOWNLOAD_ICON_X 20
#define POS_NET_DOWNLOAD_ICON_Y 70
#define POS_NET_UPLOAD_TEXT_X 50
#define POS_NET_UPLOAD_TEXT_Y 35
#define POS_NET_DOWNLOAD_TEXT_X 50
#define POS_NET_DOWNLOAD_TEXT_Y 70
#define NET_TEXT_WIDTH 80
#define NET_TEXT_HEIGHT 20
#define NET_TEXT_SIZE 2

#define POS_STOCK_LABEL_X 20
#define POS_STOCK_LABEL_Y 125
#define STOCK_LABEL_WIDTH 110
#define STOCK_LABEL_HEIGHT 30
#define STOCK_LABEL_SIZE 2
#define POS_STOCK_PRICE_X 15
#define POS_STOCK_PRICE_Y 170
#define STOCK_PRICE_WIDTH 120
#define STOCK_PRICE_HEIGHT 40
#define STOCK_PRICE_SIZE 4

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