#ifndef MC_SERIAL_H
#define MC_SERIAL_H

#include <map>
#include <WiFiManager.h>
#include "../mclcd/mclcd.h"
#include "../mcwifi/mcwifi.h"

struct SerialCommand {
  String key;
  String title;
  String inputTip;
};

class McSerial {
  private:
    String SMOD;
    std::map<String, SerialCommand> serialCommandMap;
    void initCommandMap();
    void serialLog(String txt);
  public:
    McSerial();
    void serialLoop();
};

#endif