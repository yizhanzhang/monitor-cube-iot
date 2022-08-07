#include "mcloading.h"

extern TFT_eSprite clk;

const uint8_t *loadImgArr[] = {
  ii0,  ii2,   ii4,   ii6,  ii8,
  ii10, ii12,  ii14,  ii16, ii18,
  ii20, ii22,  ii24,  ii26, ii28, 
  ii30, ii32,  ii34, 
};

uint32_t loadImgSizeArr[] = {
  sizeof(ii0),  sizeof(ii2),  sizeof(ii4),  sizeof(ii6),  sizeof(ii8),
  sizeof(ii10), sizeof(ii12), sizeof(ii14), sizeof(ii16), sizeof(ii18),
  sizeof(ii20), sizeof(ii22), sizeof(ii24), sizeof(ii26), sizeof(ii28),
  sizeof(ii30), sizeof(ii32), sizeof(ii34),
};

McLoading::McLoading(void) {
  animLastTimeStamp = 0;
  animLoopCounter = 0;
};

void McLoading::drawText() {
  // 文字部分
  clk.setColorDepth(8);
  clk.createSprite(200, 130);//创建窗口
  clk.fillSprite(0x0000);   //填充率
  clk.setTextDatum(CC_DATUM);   //设置文本数据
  clk.setTextColor(TFT_ORANGE, 0x0000);
  clk.drawString("Monitor Cube!", 100, 30, 4);
  clk.setTextColor(TFT_WHITE, 0x0000);
  clk.drawRightString(DESC, 200, 90, 1);
  clk.pushSprite(20, 130); //窗口位置
  clk.deleteSprite();
};

void McLoading::drawAnim() {
  if (millis() - animLastTimeStamp < ANIM_DELTA) {
    return void();
  }

  // 更新记录值
  animLoopCounter ++;
  animLastTimeStamp = millis();
  
  /* 绘制动图和文案提示 */
  // 动图部分
  int animCounter = animLoopCounter % 18;
  if (animCounter < 18) {
    TJpgDec.drawJpg(IMG_X, IMG_Y, loadImgArr[animCounter], loadImgSizeArr[animCounter]);
  } else {
    Serial.println("显示AnimLoad错误");
  }
};

void McLoading::loading() {
  drawText();

  // reset counter
  animLoopCounter = 0;
  while (WiFi.status() != WL_CONNECTED && animLoopCounter < ANIM_LOOP_MIN) {
    drawAnim();
    delay(ANIM_DELTA);
  }

  if (animLoopCounter >= ANIM_LOOP_MIN) {
    Serial.println("WIFI链接超时");
  }

  // 至少循环ANIM_LOOP_MIN次
  while (animLoopCounter < ANIM_LOOP_MIN) {
    drawAnim();
    delay(ANIM_DELTA);
  }
};