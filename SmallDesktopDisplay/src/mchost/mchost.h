#ifndef MC_HOST_H
#define MC_HOST_H

#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>

#include "../../img/host/i0.h"
#include "../../img/host/i1.h"

struct HostInfo {
  int cpuData;
  int memData;
  float netUploadData;
  float netDownloadData;
};

class McHost {
  private:
    HostInfo hostInfo;
    void fetch();
    void draw();
  public:
    McHost();
    void init();
    void update();
};

# endif