#include "mcloading.h"

extern TFT_eSprite clk;

McLoading::McLoading(void) {
  animCounter = 0;
  animLastTimeStamp = 0;
  animLoopCounter = 0;
};

void McLoading::draw() {
  if (millis() - animLastTimeStamp < ANIM_DELTA) {
    return void();
  }

  // 更新记录值
  animCounter ++;
  if (animCounter >= 36) {
    animCounter = 0;
  }
  animLoopCounter ++;
  animLastTimeStamp = millis();
  
  /* 绘制动图和文案提示 */
  // 初始化Sprite
  clk.setColorDepth(8);
  clk.createSprite(200, 130);//创建窗口
  clk.fillSprite(0x0000);   //填充率
  // 动图部分
  switch (animCounter)
  {
    case 0:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii0, sizeof(ii0));
      break;
    case 1:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii1, sizeof(ii1));
      break;
    case 2:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii2, sizeof(ii2));
      break;
    case 3:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii3, sizeof(ii3));
      break;
    case 4:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii4, sizeof(ii4));
      break;
    case 5:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii5, sizeof(ii5));
      break;
    case 6:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii6, sizeof(ii6));
      break;
    case 7:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii7, sizeof(ii7));
      break;
    case 8:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii8, sizeof(ii8));
      break;
    case 9:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii9, sizeof(ii9));
      break;
    case 10:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii10, sizeof(ii10));
      break;
    case 11:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii11, sizeof(ii11));
      break;
    case 12:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii12, sizeof(ii12));
      break;
    case 13:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii13, sizeof(ii13));
      break;
    case 14:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii14, sizeof(ii14));
      break;
    case 15:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii15, sizeof(ii15));
      break;
    case 16:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii16, sizeof(ii16));
      break;
    case 17:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii17, sizeof(ii17));
      break;
    case 18:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii18, sizeof(ii18));
      break;
    case 19:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii19, sizeof(ii19));
      break;
    case 20:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii20, sizeof(ii20));
      break;
    case 21:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii21, sizeof(ii21));
      break;
    case 22:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii22, sizeof(ii22));
      break;
    case 23:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii23, sizeof(ii23));
      break;
    case 24:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii24, sizeof(ii24));
      break;
    case 25:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii25, sizeof(ii25));
      break;
    case 26:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii26, sizeof(ii26));
      break;
    case 27:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii27, sizeof(ii27));
      break;
    case 28:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii28, sizeof(ii28));
      break;
    case 29:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii29, sizeof(ii29));
      break;
    case 30:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii30, sizeof(ii30));
      break;
    case 31:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii31, sizeof(ii31));
      break;
    case 32:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii32, sizeof(ii32));
      break;
    case 33:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii33, sizeof(ii33));
      break;
    case 34:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii34, sizeof(ii34));
      break;
    case 35:
      TJpgDec.drawJpg(IMG_X, IMG_Y, ii35, sizeof(ii35));
      break;
      
      /*
      */
    default:
      Serial.println("显示AnimLoad错误");
      break;
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

bool McLoading::loading() {
  while (WiFi.status() != WL_CONNECTED && animLoopCounter < ANIM_LOOP_MIN) {
    draw();
    delay(ANIM_DELTA);
  }

  // 至少循环ANIM_LOOP_MIN次
  while (animLoopCounter < ANIM_LOOP_MIN) {
    draw();
    delay(ANIM_DELTA);
  }

  return WiFi.status() != WL_CONNECTED;
};