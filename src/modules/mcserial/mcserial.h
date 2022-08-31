#ifndef MC_SERIAL_H
#define MC_SERIAL_H

#include <map>
#include "../mclcd/mclcd.h"
#include "../mctheme/mchost/mchost.h"

struct SerialCommand {
  String key;
  String title;
  String inputTip;
  void   (*handle)(String);
};

class McSerial {
  private:
    std::map<String, SerialCommand> serialCommandMap;
    void initCommandMap();
  public:
    McSerial();
    void tick();
};

#endif