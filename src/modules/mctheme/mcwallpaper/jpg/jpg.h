#ifndef JPG_H
#define JPG_H

#include <Arduino.h>
#include <FS.h>
#include <TFT_eSPI.h>
#include <JPEGDecoder.h>

#define minimum(a,b) (((a) < (b)) ? (a) : (b))

void drawJpeg(String filename, int xpos, int ypos);

#endif