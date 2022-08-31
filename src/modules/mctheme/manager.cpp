#include "manager.h"

extern McHost mcHost;
extern McWallpaper mcWallpaper;

McThemeManager::McThemeManager() {
  counter = 0;
	themeVector.push_back(&mcHost);
  themeVector.push_back(&mcWallpaper);
}

void McThemeManager::initTheme() {
  themeVector.at(0)->init();
}

void McThemeManager::switchTheme() {
  counter = (counter + 1) % themeVector.size();
  themeVector.at(counter)->init();
}

void McThemeManager::tick() {
  themeVector.at(counter)->update();
}
