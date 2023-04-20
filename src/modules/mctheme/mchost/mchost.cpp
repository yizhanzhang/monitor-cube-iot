#include "mchost.h"

extern TFT_eSprite clk;
extern TFT_eSPI tft;

StaticJsonDocument<300> doc;

void drawMainImage(const uint8_t image[], uint32_t size) {
  TJpgDec.drawJpg(POS_ERROR_X, POS_ERROR_Y, image, size);
}

void drawPercentColumn(int32_t x, int32_t y, int innerHeight, uint32_t color) {
  // draw bar
  clk.createSprite(COLUMN_OUTER_WIDTH, COLUMN_OUTER_HEIGHT);
  clk.fillSprite(0x0000);
  clk.fillRoundRect(0, 0, COLUMN_OUTER_WIDTH, COLUMN_OUTER_HEIGHT, COLUMN_OUTER_RADIUS, TFT_WHITE);
  clk.fillRoundRect(COLUMN_OUTER_THICKNESS, COLUMN_OUTER_THICKNESS, COLUMN_OUTER_WIDTH - 2 * COLUMN_OUTER_THICKNESS, COLUMN_OUTER_HEIGHT - 2 * COLUMN_OUTER_THICKNESS, COLUMN_OUTER_RADIUS - COLUMN_OUTER_THICKNESS, TFT_BLACK);
  // draw fill
  if (innerHeight > 0) {
    int leftOffset = (COLUMN_OUTER_WIDTH - COLUMN_INNER_WIDTH) / 2;
    int topOffset = (COLUMN_OUTER_HEIGHT - COLUMN_INNER_HEIGHT) / 2 + (COLUMN_INNER_HEIGHT - innerHeight);
    clk.fillRoundRect(leftOffset, topOffset, COLUMN_INNER_WIDTH, innerHeight, COLUMN_INNER_RADIUS, color);
  }
  // push to screen
  clk.pushSprite(x, y);
  clk.deleteSprite();
}

void drawTextString(int32_t x, int32_t y, int width, int height, String text, int fontSize, uint32_t color, uint32_t bgColor = TFT_BLACK, uint32_t datum = MC_DATUM) {
  clk.createSprite(width, height);
  clk.fillSprite(bgColor);
  clk.setTextColor(color);
  clk.setTextDatum(datum);
  // support wechat font for 20 size
  if (fontSize == 4) {
    clk.setFreeFont(MYFONT20);
    clk.setTextSize(1);
  } else {
    clk.setTextFont(1);
    clk.setTextSize(fontSize);
  }
  int textOffsetX = 0;
  int textOffsetY = 0;
  if (datum == MC_DATUM) {
    textOffsetX = width / 2;
    textOffsetY = height / 2;
  } else if (datum == ML_DATUM) {
    textOffsetX = 0;
    textOffsetY = height / 2;
  }
  clk.drawString(text, textOffsetX, textOffsetY, 1);
  clk.pushSprite(x, y);
  clk.deleteSprite();
}

McHost::McHost(void) {
  hostInfo = HostInfo{ 0, 0, 0, "00.00M", "00.00M", 0, "unknown", "000.0"};
  timestamp = 0;
};

void McHost::init() {
  Serial.println("McHost::init");
};

void McHost::update() {
  int nowStamp = millis();

  if (nowStamp - timestamp > RENDER_TIME_GAP) {
    timestamp = millis();
    drawInfo();
  }
  // 兼容milles极限
  if (nowStamp < timestamp) {
    timestamp = millis();
  }
};

void McHost::drawInfo() {
  // 检查信息是否还有效
  bool nowInvalid = timestamp - hostInfo.timestamp > INFO_VALID_GAP;
  if (nowInvalid != isInvalid) {
    tft.fillScreen(TFT_BLACK);
    isInvalid = nowInvalid;
  }
  if (isInvalid) {
    drawMainImage(unlink_120X120, sizeof(unlink_120X120));
    return;
  }

  // 绘制CPU和MEM
  int cpuHeight = hostInfo.cpuData * COLUMN_INNER_HEIGHT / 100;
  drawPercentColumn(POS_CPU_COLUMN_X, POS_CPU_COLUMN_Y, cpuHeight, TFT_CYAN);
  TJpgDec.drawJpg(POS_CPU_ICON_X, POS_CPU_ICON_Y, icpu_20X20, sizeof(icpu_20X20));
  int memHeight = hostInfo.memData * COLUMN_INNER_HEIGHT / 100;
  drawPercentColumn(POS_MEM_COLUMN_X, POS_MEM_COLUMN_Y, memHeight, TFT_GREEN);
  TJpgDec.drawJpg(POS_MEM_ICON_X, POS_MEM_ICON_Y, imem_20X20, sizeof(imem_20X20));

  // 绘制NET
  TJpgDec.drawJpg(POS_NET_UPLOAD_ICON_X, POS_NET_UPLOAD_ICON_Y, iupload_20X20, sizeof(iupload_20X20));
  drawTextString(POS_NET_UPLOAD_TEXT_X, POS_NET_UPLOAD_TEXT_Y, NET_TEXT_WIDTH, NET_TEXT_HEIGHT, hostInfo.netUploadData, NET_TEXT_SIZE, TFT_ORANGE, ML_DATUM);
  TJpgDec.drawJpg(POS_NET_DOWNLOAD_ICON_X, POS_NET_DOWNLOAD_ICON_Y, idownload_20X20, sizeof(idownload_20X20));
  drawTextString(POS_NET_DOWNLOAD_TEXT_X, POS_NET_DOWNLOAD_TEXT_Y, NET_TEXT_WIDTH, NET_TEXT_HEIGHT, hostInfo.netDownloadData, NET_TEXT_SIZE, TFT_ORANGE, ML_DATUM);

  // 绘制STOCK
  drawTextString(POS_STOCK_LABEL_X, POS_STOCK_LABEL_Y, STOCK_LABEL_WIDTH, STOCK_LABEL_HEIGHT, hostInfo.stockName, STOCK_LABEL_SIZE, TFT_WHITE, TFT_BLUE);
  int priceColorArr[3] = { TFT_LIGHTGREY, TFT_RED, TFT_GREEN };
  drawTextString(POS_STOCK_PRICE_X, POS_STOCK_PRICE_Y, STOCK_PRICE_WIDTH, STOCK_PRICE_HEIGHT, hostInfo.stockData, STOCK_PRICE_SIZE, priceColorArr[hostInfo.stockStatus]);
};

void McHost::updateInfo(String str) {
  /* 解析数据 */
  DeserializationError error = deserializeJson(doc, str);
  //检查反序列化是否成功
  if (!error) {
    hostInfo.timestamp = millis();
    hostInfo.cpuData = doc["cpuData"];
    hostInfo.memData = doc["memData"];
    const char *netUploadData = doc["downloadData"];
    hostInfo.netUploadData = netUploadData;
    const char *netDownloadData = doc["uploadData"];
    hostInfo.netDownloadData = netDownloadData;
    hostInfo.stockStatus = doc["stockStatus"];
    const char *stockName= doc["stockName"];
    hostInfo.stockName = stockName;
    const char *stockData = doc["stockData"];
    hostInfo.stockData = stockData;
  } else {
    hostInfo.timestamp = millis();
    hostInfo.cpuData = hostInfo.memData = hostInfo.stockStatus = 0;
    hostInfo.netUploadData = hostInfo.netDownloadData = hostInfo.stockName = hostInfo.stockData = = "";
  }
};