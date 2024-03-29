#ifndef MC_MANAGER_H
#define MC_MANAGER_H

#include <vector>
#include "base.h"
#include "mchost/mchost.h"

class McThemeManager {
  private:
    int counter;
    std::vector<McThemeBase*> themeVector;
  public:
    McThemeManager();
    void initTheme();
    void switchTheme();
    void clickTheme();
    void tick();
};

#endif