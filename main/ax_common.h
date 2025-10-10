#ifndef __AX_COMMON_H__
#define __AX_COMMON_H__

#include <Arduino.h>
// #include <esp_task_wdt.h>
#include <esp_mac.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>

#include "ax_ssd1306.h"
#include "ax_gpio.h"
#include "ax_flash.h"

#define ERRCODEARR_DOC_LEN (1024)


// extern StaticJsonDocument<ERRCODEARR_DOC_LEN> errorCodeArray;
extern JsonDocument errorCodeArray;

extern String VERSION;

/*
 * 修改固件需要修改以下宏：
 *    VERSION_NUM - 版本号
 */
#define VERSION_NUM "2.0.0.1"

#define VERSION_CN_LOC "-cn_local"
#define VERSION_EN_LOC "-en_local"
#define VERSION_JP_LOC "-jp_local"
#define VERSION_CN_DEV "-cn_dev"
#define VERSION_EN_DEV "-en_dev"
#define VERSION_JP_DEV "-jp_dev"
#define VERSION_CN "-cn"
#define VERSION_EN "-en"
#define VERSION_JP "-jp"
#define VERSION_EN_INTERNATIONAL "-en-international"
#define VERSION_CN_INTERNATIONAL "-cn-international"
#define VERSION_JAPANESE "-jp-international"

// u8g2_font_b10_t_japanese1
// u8g2_font_b10_b_t_japanese1
// u8g2_font_b12_t_japanese1
// u8g2_font_b12_b_t_japanese1
// u8g2_font_b16_t_japanese1
// u8g2_font_b16_b_t_japanese1
// u8g2_font_unifont_t_japanese2
// u8g2_font_wqy12_t_gb2312a
#define SET_DISPLAY_SN u8g2_font_wqy16_t_gb2312a

#endif
