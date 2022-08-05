#include "mcloading.h"

extern TFT_eSprite clk;

const uint8_t *loadImgArr[] = {
  ii0,  ii1,  ii2,  ii3,  ii4,  ii5,
  ii6,  ii7,  ii8,  ii9,  ii10, ii11,
  ii12, ii13, ii14, ii15, ii16, ii17,
  ii18, ii19, ii20, ii21, ii22, ii23,
  ii24, ii25, ii26, ii27, ii28, ii29,
  ii30, ii31, ii32, ii33, ii34, ii35,
};

uint32_t loadImgSizeArr[] = {
  sizeof(ii0),  sizeof(ii1),  sizeof(ii2),  sizeof(ii3),  sizeof(ii4),  sizeof(ii5),
  sizeof(ii6),  sizeof(ii7),  sizeof(ii8),  sizeof(ii9),  sizeof(ii10), sizeof(ii11),
  sizeof(ii12), sizeof(ii13), sizeof(ii14), sizeof(ii15), sizeof(ii16), sizeof(ii17),
  sizeof(ii18), sizeof(ii19), sizeof(ii20), sizeof(ii21), sizeof(ii22), sizeof(ii23),
  sizeof(ii24), sizeof(ii25), sizeof(ii26), sizeof(ii27), sizeof(ii28), sizeof(ii29),
  sizeof(ii30), sizeof(ii31), sizeof(ii32), sizeof(ii33), sizeof(ii34), sizeof(ii35),
};

McLoading::McLoading(void) {
  animLastTimeStamp = 0;
  animLoopCounter = 0;
};

void McLoading::draw() {
  if (millis() - animLastTimeStamp < ANIM_DELTA) {
    return void();
  }

  // 更新记录值
  animLoopCounter ++;
  animLastTimeStamp = millis();
  
  /* 绘制动图和文案提示 */
  // 初始化Sprite
  clk.setColorDepth(8);
  clk.createSprite(200, 130);//创建窗口
  clk.fillSprite(0x0000);   //填充率
  // 动图部分
  int animCounter = animLoopCounter % 36;
  if (animCounter < 36) {
    TJpgDec.drawJpg(IMG_X, IMG_Y, loadImgArr[animCounter], loadImgSizeArr[animCounter]);
  } else {
    Serial.println("显示AnimLoad错误");
  }
  // 文字部分
  clk.setTextDatum(CC_DATUM);   //设置文本数据
  clk.setTextColor(TFT_ORANGE, 0x0000);
  clk.drawString("Monitor Cube!", 100, 30, 4);
  clk.setTextColor(TFT_WHITE, 0x0000);
  clk.drawRightString(DESC, 200, 90, 1);
  clk.pushSprite(20, 130); //窗口位置

  clk.deleteSprite();
};

void McLoading::loading() {
  // reset counter
  animLoopCounter = 0;

  while (WiFi.status() != WL_CONNECTED && animLoopCounter < ANIM_LOOP_MIN) {
    draw();
    delay(ANIM_DELTA);
  }

  // 至少循环ANIM_LOOP_MIN次
  while (animLoopCounter < ANIM_LOOP_MIN) {
    draw();
    delay(ANIM_DELTA);
  }
};