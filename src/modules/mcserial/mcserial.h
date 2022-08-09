#ifndef MC_SERIAL_H
#define MC_SERIAL_H

#include <string>
#include <map>
#include <WiFiManager.h>
#include "../mclcd/mclcd.h"
#include "../mcwifi/mcwifi.h"

using namespace std;

struct SerialCommand {
  string id;
  string title;
};

class McSerial {
  private:
    std::map<string, SerialCommand> mapSerialCommand;
    String SMOD;
  public:
    McSerial();
    void serialLoop();
};

#endif