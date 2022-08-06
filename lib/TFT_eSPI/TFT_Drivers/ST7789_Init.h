
// This is the command sequence that initialises the ST7789 driver
//
// This setup information uses simple 8 bit SPI writecommand() and writedata() functions
//
// See ST7735_Setup.h file for an alternative format

{
  writecommand(ST7789_SLPOUT);   // Sleep out
  delay(120);

  writecommand(ST7789_NORON);    // 正常显示模式开启

  //------------------------------显示和颜色格式设置--------------------------------//
  writecommand(ST7789_MADCTL);
  //writedata(0x00);
  writedata(TFT_MAD_COLOR_ORDER);

  // JLX240 显示器数据表
  writecommand(0xB6);
  writedata(0x0A);
  writedata(0x82);

  writecommand(ST7789_COLMOD);
  writedata(0x55);
  delay(10);

  //--------------------------------ST7789V 帧率设置----------------------------------//
  writecommand(ST7789_PORCTRL);
  writedata(0x0c);
  writedata(0x0c);
  writedata(0x00);
  writedata(0x33);
  writedata(0x33);

  writecommand(ST7789_GCTRL);      // 电压：VGH / VGL
  writedata(0x35);

  //---------------------------------ST7789V 电源设置--------------------------------------//
  writecommand(ST7789_VCOMS);
  writedata(0x28);		// JLX240 显示器数据表

  writecommand(ST7789_LCMCTRL);
  writedata(0x0C);

  writecommand(ST7789_VDVVRHEN);
  writedata(0x01);
  writedata(0xFF);

  writecommand(ST7789_VRHS);       // 电压 VRHS
  writedata(0x10);

  writecommand(ST7789_VDVSET);
  writedata(0x20);

  writecommand(ST7789_FRCTR2);
  writedata(0x0f);

  writecommand(ST7789_PWCTRL1);
  writedata(0xa4);
  writedata(0xa1);

  //--------------------------------ST7789V 伽马设置---------------------------------------//
  writecommand(ST7789_PVGAMCTRL);
  writedata(0xd0);
  writedata(0x00);
  writedata(0x02);
  writedata(0x07);
  writedata(0x0a);
  writedata(0x28);
  writedata(0x32);
  writedata(0x44);
  writedata(0x42);
  writedata(0x06);
  writedata(0x0e);
  writedata(0x12);
  writedata(0x14);
  writedata(0x17);

  writecommand(ST7789_NVGAMCTRL);
  writedata(0xd0);
  writedata(0x00);
  writedata(0x02);
  writedata(0x07);
  writedata(0x0a);
  writedata(0x28);
  writedata(0x31);
  writedata(0x54);
  writedata(0x47);
  writedata(0x0e);
  writedata(0x1c);
  writedata(0x17);
  writedata(0x1b);
  writedata(0x1e);

  writecommand(ST7789_INVON);

  writecommand(ST7789_CASET);    // Column address set
  writedata(0x00);
  writedata(0x00);
  writedata(0x00);
  writedata(0xE5);    // 239

  writecommand(ST7789_RASET);    // Row address set
  writedata(0x00);
  writedata(0x00);
  writedata(0x01);
  writedata(0x3F);    // 319

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  end_tft_write();
  delay(120);
  begin_tft_write();
  TFT_eSPI::fillScreen(0x0000);
  delay(100);
  writecommand(ST7789_DISPON);    //开显示

#ifdef TFT_BL
  // Turn on the back-light LED
  digitalWrite(TFT_BL, HIGH);
  pinMode(TFT_BL, OUTPUT);
#endif
}
