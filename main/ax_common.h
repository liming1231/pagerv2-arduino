#ifndef __AX_COMMON_H__
#define __AX_COMMON_H__

#include <Arduino.h>
// #include <esp_task_wdt.h>
#include <esp_mac.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>
#include <PubSubClient.h>

#include "ax_ssd1306.h"
#include "ax_gpio.h"
#include "ax_webserver.h"
#include "ax_updater.h"
#include "ax_flash.h"

#define ERRCODEARR_DOC_LEN (1024)


// extern StaticJsonDocument<ERRCODEARR_DOC_LEN> errorCodeArray;
extern JsonDocument errorCodeArray;
extern JsonDocument web_sts;

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

#define AP_GW_1 "10.25.25.1"
#define AP_GW_2 "172.25.25.1"

#define KEY(o) "\"" #o "\":"
#define VALUE_STR(o) "\"" #o "\""

#define KEY_1(o) "\"" o "\":"
#define VALUE_STR_1(o) "\"" o "\""
#define VALUE_DEF(o) (String("\"") + o + "\"")

typedef enum {
  NANN = 0,
  DOCALL = 1,
  CALLSUCCFB = 2,
  CALLFAILFB = 3,
  DOCANCEL = 4,
  CANCELSUCCFB = 5,
  CANCELFAILFB = 6,
  DOCFG = 7,
  CFGSUCCFB = 8,
  CFGFAILFB = 9,
  CFGSUCC = 10,
  QUEUING = 11,
  ALLOCATE = 12,
  TASKCREATE = 13,
  TASKSTART = 14,
  TASKSUCC = 15,
  TASKFAILED = 16,
  TASKCANCEL = 17,
  WIFICFG = 18,
  WIFICFGSUCC = 19,
  NOTICE_GATEWAY = 20,
  ACCIDENT = 21,
  DEPLOY_ENV_SUCC = 22,
  DEPLOY_ENV_FAILED = 23,
  FACTORY_SETTINGS = 24

} DISPLAY_TYPE;

// u8g2_font_b10_t_japanese1
// u8g2_font_b10_b_t_japanese1
// u8g2_font_b12_t_japanese1
// u8g2_font_b12_b_t_japanese1
// u8g2_font_b16_t_japanese1
// u8g2_font_b16_b_t_japanese1
// u8g2_font_unifont_t_japanese2
// u8g2_font_wqy12_t_gb2312a
#define SET_DISPLAY_SN u8g2_font_wqy16_t_gb2312a

typedef struct _cfg_wifi {
  bool Static_ip;
  String Static_ip_addr;
  String Static_gateway;
  String Static_subnet;
  String dns;
  String proto;
  String input_SSID;
  String input_Password;
  String method;
  String wpa2_identity;
  String wpa2_username;
  String deploy_env;
  String dagreement_type;
  String priv_serv_addr;
  int32_t priv_serv_port;
  String mqtt_priv_usr;
  String mqtt_priv_pwd;
  String deploy_lang;
  String web_lang;
} cfg_wifi;

extern cfg_wifi cfgWifi;

extern PubSubClient client;

#endif
