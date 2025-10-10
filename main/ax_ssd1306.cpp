#include "ax_common.h"

uint8_t mac[6] = { 0 };
char mac_s[16] = { 0 };

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock=*/10, /* data=*/9);

display_field displayField;


static const unsigned char bitmap_bit_bytes[] U8X8_PROGMEM = {
  /* (120 X 43 )*/
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00,
  0x00, 0x3F, 0x20, 0x80, 0xFC, 0x0F, 0x3E, 0x78, 0xE0, 0x63, 0x38, 0x60, 0xF0, 0x83, 0x03,
  0xC0, 0x7F, 0x30, 0xC0, 0xFC, 0x8F, 0x7F, 0xF0, 0xF0, 0x71, 0x3C, 0x70, 0xFC, 0xC7, 0x01,
  0xC0, 0xFF, 0x38, 0xE0, 0xF8, 0xC7, 0xFF, 0xF0, 0xF9, 0x70, 0x7E, 0x70, 0xFE, 0x63, 0x00,
  0xE0, 0xE1, 0x39, 0xE0, 0xE0, 0xE0, 0xC1, 0xE1, 0x73, 0x70, 0xFE, 0x70, 0x1E, 0x10, 0x00,
  0xE0, 0xC0, 0x39, 0xE0, 0xC0, 0xE0, 0xC0, 0xC1, 0x27, 0x70, 0xFE, 0x70, 0x0F, 0x00, 0x00,
  0x70, 0xCE, 0x39, 0xE0, 0xC0, 0x60, 0x80, 0x83, 0x07, 0x70, 0xFE, 0x31, 0x07, 0x00, 0x00,
  0x70, 0x9E, 0x39, 0xE0, 0xC0, 0x60, 0x80, 0x03, 0x0F, 0x70, 0xCE, 0x33, 0x07, 0x1F, 0x00,
  0x70, 0x8E, 0x39, 0xE0, 0xC0, 0x60, 0x80, 0x83, 0x0F, 0x70, 0x9E, 0x33, 0x07, 0x1E, 0x00,
  0x70, 0x80, 0x71, 0xE0, 0xC0, 0xE0, 0xC0, 0xC1, 0x27, 0x70, 0x1E, 0x7F, 0x07, 0x1C, 0x00,
  0x70, 0x80, 0xF1, 0x70, 0xC0, 0xE0, 0xC1, 0xE1, 0x73, 0x70, 0x1E, 0x7F, 0x0E, 0x1E, 0x00,
  0x70, 0x80, 0xE1, 0x7F, 0xC0, 0xC0, 0xFF, 0xF0, 0xF9, 0x70, 0x1E, 0x7E, 0xFE, 0x0F, 0x00,
  0x70, 0x80, 0xE1, 0x3F, 0xC0, 0x80, 0xFF, 0xF0, 0xF0, 0x71, 0x0E, 0x3C, 0xFC, 0x07, 0x00,
  0x30, 0x80, 0x80, 0x0F, 0x40, 0x00, 0x3F, 0x78, 0xE0, 0x33, 0x06, 0x1C, 0xF0, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xE0, 0xFF, 0x01, 0x00, 0x20, 0xFF, 0x00, 0x00, 0x7C, 0x3E, 0x00, 0x00, 0x80, 0x01, 0x00,
  0xE0, 0x80, 0x03, 0x00, 0x30, 0xFF, 0x00, 0x00, 0x7C, 0x3E, 0x00, 0x80, 0xFF, 0x0F, 0x00,
  0xE0, 0xFF, 0x03, 0x00, 0x18, 0x00, 0x00, 0x00, 0x7E, 0x3F, 0x00, 0x00, 0x80, 0x1D, 0x00,
  0x60, 0x80, 0x03, 0x00, 0x70, 0x00, 0x00, 0x00, 0x7C, 0x3E, 0x00, 0x00, 0x80, 0x1D, 0x00,
  0xE0, 0xFF, 0x03, 0x00, 0x30, 0xFF, 0x00, 0x00, 0xFE, 0x3F, 0x00, 0x80, 0x9F, 0x1D, 0x00,
  0x00, 0x1C, 0x00, 0x00, 0x38, 0xFF, 0x00, 0x00, 0xFE, 0x1F, 0x00, 0x00, 0x9F, 0x1D, 0x00,
  0xE0, 0xFF, 0x03, 0x00, 0x38, 0x60, 0x00, 0x00, 0xFC, 0x3F, 0x00, 0x00, 0x83, 0x1D, 0x00,
  0x60, 0xFE, 0x00, 0x00, 0x30, 0x60, 0x00, 0x00, 0xFC, 0x3F, 0x00, 0x00, 0x83, 0x1D, 0x00,
  0xE0, 0xFF, 0x03, 0x00, 0x30, 0x60, 0x00, 0x00, 0xFC, 0x3F, 0x00, 0x00, 0xDB, 0x1C, 0x00,
  0x60, 0x00, 0x03, 0x00, 0x30, 0x60, 0x00, 0x00, 0xFE, 0x3F, 0x00, 0x00, 0xDB, 0x1C, 0x00,
  0xE0, 0xFF, 0x03, 0x00, 0x30, 0x60, 0x00, 0x00, 0x6C, 0x1B, 0x00, 0x00, 0xDB, 0x1C, 0x00,
  0xC0, 0x9C, 0x00, 0x00, 0x30, 0x60, 0x00, 0x00, 0x7E, 0x3E, 0x00, 0x80, 0xDB, 0x1C, 0x00,
  0xE0, 0x9C, 0x03, 0x00, 0x30, 0x7C, 0x00, 0x00, 0xF6, 0x37, 0x00, 0x80, 0x7F, 0x0E, 0x00,
  0x60, 0x0E, 0x03, 0x00, 0x30, 0x3C, 0x00, 0x00, 0xE2, 0x37, 0x00, 0x00, 0x3F, 0x0E, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void show_logo(void)
{
    u8g2.drawXBMP(0, 16, 120, 43, bitmap_bit_bytes);
    u8g2.sendBuffer();
}

/* 设置显示数据的起始点
 * goline - 显示屏行号，1-4，每行8个像素
 * posx   - 显示屏像素点列，0-128
 */
void set_start_position(uint8_t goline, uint8_t posx)
{
    switch (goline)
    {
    case LINE_1:
    {
        u8g2.setCursor(posx, 14);
        break;
    }
    case LINE_2:
    {
        u8g2.setCursor(posx, 30);
        break;
    }
    case LINE_3:
    {
        u8g2.setCursor(posx, 46);
        break;
    }
    case LINE_4:
    {
        u8g2.setCursor(posx, 62);
        break;
    }
    default:
    {
        break;
    }
    }
}


/*
 *   初始化显示屏
 */
void init_ssd1306(void) {
  u8g2.begin();
  u8g2.enableUTF8Print();
  // use chinese2 for all the glyphs
  if (displayVars.user_lang == USE_CHINESE_LANG) {
    u8g2.setFont(u8g2_font_wqy16_t_gb2312a);
  } else if (displayVars.user_lang == USE_JAPANESE_LANG) {
    u8g2.setFont(u8g2_font_b12_t_japanese3);
  } else if (displayVars.user_lang == USE_ENGLISH_LANG) {
    u8g2.setFont(u8g2_font_wqy16_t_gb2312a);
  } else {
    u8g2.setFont(u8g2_font_wqy16_t_gb2312a);
  }

  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  if (varData.isNeutralLogo == 1) {
    u8g2.setFont(SET_DISPLAY_SN);

    set_start_position(LINE_2, 10);
    u8g2.print("ROBOT PAGER");
    u8g2.sendBuffer();
  } else {
    if (displayVars.user_lang == USE_JAPANESE_LANG) {
      u8g2.setFont(SET_DISPLAY_SN);
      set_start_position(LINE_2, 30);
      u8g2.print(displayField.firstScreen);
      u8g2.sendBuffer();
    } else {
      show_logo();
    }
  }
}


void display_test(void) {
  u8g2.clearBuffer();  // clear the internal memory
  // u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  u8g2.drawStr(0, 14, "ID:");   // write something to the internal memory
  u8g2.drawStr(24, 14, mac_s);  // write something to the internal memory
  u8g2.setCursor(0, 30);
  u8g2.print("你好");
  u8g2.sendBuffer();  // transfer internal memory to the display
}

/*
 *   初始化错误码表
 *
 */
void init_error_code(void) {
  if (displayVars.user_lang == USE_ENGLISH_LANG) {
    errorCodeArray["1001"] = "cfg gateway";
    errorCodeArray["1002"] = "cfg pager";
    errorCodeArray["1003"] = "call repetition";
    errorCodeArray["1004"] = "unbound business";
    errorCodeArray["1005"] = "unbound robot";
    errorCodeArray["1006"] = "unbound POI";

    errorCodeArray["2001"] = "robot cannot move";
    errorCodeArray["2002"] = "robot e-stopped";
    errorCodeArray["2003"] = "robot recharge";
    errorCodeArray["2004"] = "manual ctrl mode";
    errorCodeArray["2005"] = "remote ctrl mode";
    errorCodeArray["2006"] = "move blocked";
    errorCodeArray["2007"] = "robot test stat";
    errorCodeArray["2008"] = "in other tasks";
    errorCodeArray["2009"] = "unexecuting task";
    errorCodeArray["2010"] = "robots unusable";
    errorCodeArray["2011"] = "task not create";
    errorCodeArray["2012"] = "task not execute";
    errorCodeArray["2013"] = "get status fail";
    errorCodeArray["2014"] = "pause timeout";

    errorCodeArray["3001"] = "robot e-stop";
    errorCodeArray["3002"] = "robot blocked";
    errorCodeArray["3003"] = "manual ctrl mode";
  } else if (displayVars.user_lang == USE_JAPANESE_LANG) {
    errorCodeArray["1001"] = "ゲートウェイの設定";
    errorCodeArray["1002"] = "呼び出し装置の設定";
    errorCodeArray["1003"] = "再呼び出し";
    errorCodeArray["1004"] = "タスクの紐付けが未完了です";
    errorCodeArray["1005"] = "ロボットとの紐付けが未完了です";
    errorCodeArray["1006"] = "位置の紐付けが未完了です";

    errorCodeArray["2001"] = "ロボットが移動できません";
    errorCodeArray["2002"] = "ロボットが緊急停止されました";
    errorCodeArray["2003"] = "ロボットが充電ステーションに戻ります";
    errorCodeArray["2004"] = "ロボットが手動モードになっています";
    errorCodeArray["2005"] = "ロボットが遠隔操作モードになっています";
    errorCodeArray["2006"] = "ロボットの移動はブロックされています";
    errorCodeArray["2007"] = "ロボットはテスト状態です";
    errorCodeArray["2008"] = "他のタスクを実行中です";
    errorCodeArray["2009"] = "ロボットはタスクを実行していません";
    errorCodeArray["2010"] = "利用可能なロボットがありません";
    errorCodeArray["2011"] = "タスクの作成に失敗しました";
    errorCodeArray["2012"] = "タスクの実行に失敗しました";
    errorCodeArray["2013"] = "ロボットの稼働状態確認に失敗しました";  //"購読状態の確立に失敗しました";
    errorCodeArray["2014"] = "タスクの一時停止に失敗しました";

    errorCodeArray["3001"] = "ロボットは緊急停止中です";
    errorCodeArray["3002"] = "障害物があります";
    errorCodeArray["3003"] = "手動モードに入ります";
  } else {
    errorCodeArray["1001"] = "网关配置";
    errorCodeArray["1002"] = "呼叫器配置";
    errorCodeArray["1003"] = "重复的呼叫";
    errorCodeArray["1004"] = "未绑定业务";
    errorCodeArray["1005"] = "未绑定机器人";
    errorCodeArray["1006"] = "未绑定POI";

    errorCodeArray["2001"] = "机器人无法移动";
    errorCodeArray["2002"] = "机器人被急停";
    errorCodeArray["2003"] = "机器人回桩充电";
    errorCodeArray["2004"] = "机器人被手动控制";
    errorCodeArray["2005"] = "机器人被远程控制";
    errorCodeArray["2006"] = "机器人移动被阻碍";
    errorCodeArray["2007"] = "机器人测试状态";
    errorCodeArray["2008"] = "执行其他任务";
    errorCodeArray["2009"] = "机器人未执行任务";
    errorCodeArray["2010"] = "没有可用机器人";
    errorCodeArray["2011"] = "任务创建失败";
    errorCodeArray["2012"] = "任务执行失败";
    errorCodeArray["2013"] = "订阅状态失败";
    errorCodeArray["2014"] = "任务暂停超时";

    errorCodeArray["3001"] = "机器人急停中";
    errorCodeArray["3002"] = "机器人被阻挡";
    errorCodeArray["3003"] = "进入手动模式";
  }
}

/*
 *   初始化用户提示信息
 *
 */
void init_display_language(void) {
  if (displayVars.user_lang == USE_ENGLISH_LANG) {
    displayField.firstScreen = "AutoXing Tech";
    displayField.unConnSts = "wifi offline:";
    displayField.connectedSts = "wifi online:";
    displayField.idleSts = "pager idle";
    displayField.cfgWifiSts = "cfg wifi";
    displayField.cfgWifiSucc = "wifi cfg succ";
    displayField.callingSts = "calling...";
    displayField.callSuccSts = "call confirmed";
    displayField.callRejectedSts = "call rejected";
    displayField.cancelCallSts = "cancel call...";
    displayField.callCanceledSts = "call canceled";
    displayField.callCancelRejected = "cancel rejected";
    displayField.positionUnbound = "poi unbound";
    displayField.positionBinding = "binding poi...";
    displayField.positionBindFail = "bind poi failed";
    displayField.positionBindSucc = "bind poi succ";
    displayField.positionBindIncomplete = "bind incomplete";
    displayField.taskWaitingSts = "waiting...";
    displayField.remainedHeader = "remain ";
    displayField.remainedTail = " task";
    displayField.remainedTails = " tasks";
    displayField.robotTag = "robot: ";
    displayField.taskTag = "taskId: ";
    displayField.rbtOnTheWay = "on the way";
    displayField.distanceHeader = "distance";
    displayField.distanceTail = "(m)";
    displayField.rbtArrived = "robot arrived";
    displayField.taskFailed = "task failed";
    displayField.accident = "task paused";
    displayField.taskCanceled = "task canceled";
    displayField.cfgDeploySucc = "deploy succ";
    displayField.cfgDeployFail = "deploy failed";
    displayField.cfg_factory_settings = "FactorySettings";
  } else if (displayVars.user_lang == USE_JAPANESE_LANG) {
    displayField.firstScreen = "RobotBank";
    displayField.unConnSts = "WIFIを接続";
    displayField.connectedSts = "WIFI接続済み";
    displayField.idleSts = "空の状態";
    displayField.cfgWifiSts = "WIFIに接続";
    displayField.cfgWifiSucc = "WIFIの設定が完了";
    displayField.callingSts = "呼び出し中です";
    displayField.callSuccSts = "呼び出し済みです";
    displayField.callRejectedSts = "呼び出しが拒否された";
    displayField.cancelCallSts = "呼び出しをキャンセル";
    displayField.callCanceledSts = "呼び出しを中止した";
    displayField.callCancelRejected = "呼び出し中止を拒否";
    displayField.positionUnbound = "位置の紐付けが未完了";
    displayField.positionBinding = "位置の紐付け。。。";
    displayField.positionBindFail = "位置の紐付けに失敗";
    displayField.positionBindSucc = "位置の紐付けに成功";
    displayField.positionBindIncomplete = "位置の紐付けが未完了";
    displayField.taskWaitingSts = "待機中";
    displayField.remainedHeader = "未完タスク残り";
    displayField.remainedTail = "個";
    displayField.remainedTails = "個";
    displayField.robotTag = "ロボット:";
    displayField.taskTag = "タスクID:";
    displayField.rbtOnTheWay = "向かっています";
    displayField.distanceHeader = "";  //"距離";
    displayField.distanceTail = "mの所にいます";
    displayField.rbtArrived = "ロボットが到着";
    displayField.taskFailed = "タスクが失敗しました";
    displayField.accident = "タスクが一時停止";
    displayField.taskCanceled = "タスクを中止した";
    displayField.cfgDeploySucc = "配備に成功しました";
    displayField.cfgDeployFail = "配備に失敗しました";
    displayField.cfg_factory_settings = "出荷時設定の復元";
  } else {
    displayField.firstScreen = "景行慧动科技";
    displayField.unConnSts = "未入网,ip:";
    displayField.connectedSts = "已入网,ip:";
    displayField.idleSts = "呼叫器空闲";
    displayField.cfgWifiSts = "请进行网络配置";
    displayField.cfgWifiSucc = "网络配置完成";
    displayField.callingSts = "正在呼叫";
    displayField.callSuccSts = "已呼叫";
    displayField.callRejectedSts = "呼叫被拒绝";
    displayField.cancelCallSts = "取消呼叫...";
    displayField.callCanceledSts = "已取消呼叫";
    displayField.callCancelRejected = "取消呼叫被拒绝";
    displayField.positionUnbound = "未绑定点位";
    displayField.positionBinding = "点位绑定...";
    displayField.positionBindFail = "点位绑定失败";
    displayField.positionBindSucc = "点位绑定成功";
    displayField.positionBindIncomplete = "未完成点位绑定";
    displayField.taskWaitingSts = "等待中...";
    displayField.remainedHeader = "还有";
    displayField.remainedTail = "个任务";
    displayField.robotTag = "机器人: ";
    displayField.taskTag = "任务ID: ";
    displayField.rbtOnTheWay = "正在赶来";
    displayField.distanceHeader = "距您";
    displayField.distanceTail = "米";
    displayField.rbtArrived = "机器人已到达";
    displayField.taskFailed = "任务失败";
    displayField.accident = "任务暂停";
    displayField.taskCanceled = "任务取消";
    displayField.cfgDeploySucc = "部署成功";
    displayField.cfgDeployFail = "部署失败";
    displayField.cfg_factory_settings = "恢复出场设置";
  }
  init_error_code();
}
