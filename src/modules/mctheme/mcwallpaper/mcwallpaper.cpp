#include "mcwallpaper.h"

McWallpaper::McWallpaper(void) {
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initial...");
    while (1) yield();
  }
  counter = 0;
  getFileAmount();
}

void McWallpaper::init() {
  drawFile();
}

void McWallpaper::update() {
}

void McWallpaper::click() {
  counter = (counter + 1) % fileAmount;
  drawFile();
}

void McWallpaper::getFileAmount() {
  fs::Dir dir = SPIFFS.openDir("/");
  int num = 0;
  while (dir.next()) {
    num += 1;
  }
  fileAmount = num;
}

void McWallpaper::listFiles(void) {
  Serial.println("SPIFFS files found:");

  fs::Dir dir = SPIFFS.openDir("/");
  String  line = "=====================================";

  Serial.println(line);
  Serial.println("  File name               Size");
  Serial.println(line);

  while (dir.next()) {
    String fileName = dir.fileName();
    Serial.print(fileName);
    int spaces = 21 - fileName.length(); // Tabulate nicely
    while (spaces--) Serial.print(" ");

    fs::File f = dir.openFile("r");
    String fileSize = (String) f.size();
    spaces = 10 - fileSize.length(); // Tabulate nicely
    while (spaces--) Serial.print(" ");
    Serial.println(fileSize + " bytes");
  }

  Serial.println(line);
  Serial.println();
  delay(1000);
};

void McWallpaper::drawFile() {
  int index = 0;
  fs::Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    if (index == counter) {
      Serial.println(dir.fileName());
      drawJpeg(dir.fileName(), 0, 0);
      break;
    }
    index++;
  }
};