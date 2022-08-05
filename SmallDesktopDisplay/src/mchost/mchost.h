#ifndef MC_HOST_H
#define MC_HOST_H

#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>

#include "../../img/host/i0.h"
#include "../../img/host/i1.h"
#include "../../img/host/download.h"
#include "../../img/host/upload.h"
#include "../../img/astronaut/i0.h"
#include "../../img/astronaut/i4.h"
#include "../../img/astronaut/i8.h"
#include "../../img/astronaut/i12.h"
#include "../../img/astronaut/i16.h"
#include "../../img/astronaut/i19.h"

#define TIME_GAP_AST 100
#define TIME_GAP_INFO 1000

struct HostInfo {
  int cpuData;
  int memData;
  String netUploadData;
  String netDownloadData;
};

class McHost {
  private:
    HostInfo hostInfo;
    int timestampAst;
    int counterAst;
    int timestampInfo;
    void drawAst();
    void drawInfo();
  public:
    McHost();
    void init();
    void update();
};

# endif