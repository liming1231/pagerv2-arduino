#ifndef __AX_FLASH_H__
#define __AX_FLASH_H__

#include <Arduino.h>
#include <nvs.h>
#include <nvs_flash.h>

// nvs storage key
#define WIFI_NAME "WIFI_NAME"
#define WIFI_PWD "WIFI_PWD"
#define WIFI_PROTO "WIFI_PROTO"
#define WIFI_METHOD "WIFI_METHOD"
#define WIFI_WPA2_ID "WPA2_ID"
#define WIFI_WPA2_USR "WPA2_USR"
#define DEVICE_ID "DEVICE_ID"
#define GATEWAY "GATEWAY"
#define ENV_TYPE_FLASH "ENV_TYPE"
#define PRIV_DEPLOY "PRIV_DEPLOY"
#define PRIV_DEPLOY_PORT "PRIV_PORT"
#define PRIV_DEPLOY_USR "PRIV_USR"
#define PRIV_DEPLOY_PWD "PRIV_PWD"
#define DIS_LANG "LANG"
#define WEB_DIS_LANG "WEB_LANG"
#define PRIV_PROTO "PRIV_PROTO"
#define HEARTBEAT_INTERVAL "HB_INTERVAL"
#define IS_AX_ENV "IS_AX_ENV"
#define DNS_CFG_FLAG "DNS_FLAG"
#define DNS_DATA "DNS_DATA"
#define AP_GATEWAY "AP_GATEWAY"
#define IS_STATICIP "IS_STATICIP"
#define STTC_IP "STTC_IP"
#define STTC_GW "STTC_GW"
#define STTC_SNT "STTC_SNT"

typedef enum
{
  UNCFG = 0,
  PRIVAT = 1,
  DEVELOPMENT = 2,
  PRODUCT = 3,
  FOREIGN = 4,
  JAPANESE = 5,
} ENV_TYPE;

typedef enum
{
  ALI_ENV = 0,
  AX_BUILD_ENV = 1,
  AX_BUILD_ENV_GLBL = 2,
  AX_BUILD_ENV_EU = 3
} AX_ENV;

typedef enum
{
  RUN_PRIVAT = 0,
  RUN_DEV_ALI = 1,
  RUN_DEV_AX_BUILD = 2,
  RUN_PROD_ALI = 3,
  RUN_PROD_AX_BUILD = 4,
  RUN_FOREIGN_ALI = 5,
  RUN_FOREIGN_AX_BUILD_GLBL = 6,
  RUN_FOREIGN_AX_BUILD_EU = 7,
  RUN_JAPANESE_ALI = 8,
  RUN_JAPANESE_AX_BUILD_GLBL = 9
} RUN_ENV;

typedef enum
{
  USE_CHINESE_LANG = 0,
  USE_ENGLISH_LANG = 1,
  USE_JAPANESE_LANG = 2,
} LANG_TYPE;

#pragma pack(1)
typedef struct
{
  char wifiName[32];
  char wifiPwd[64];
  char wifiProto[24];
  char wpa2_identity[64];
  char wpa2_username[64];
  char priv_addr[64];
  char priv_proto[8];
  uint8_t static_ip;
  uint8_t localProtoType;
  int32_t priv_port;
  char priv_usr[16];
  char priv_pwd[32];
  wpa2_auth_method_t wpa2_auth_method;
  uint8_t stationCfg;
  uint8_t deslayFlag;
  uint16_t queueLen;
  String robotId;
  float distance;
  String taskId;
  String taskEndType;
  String actionFb;
  String callStaId;
  String gatewayId;
  String rejectReason;
  String rejectReasonCode;
  String failedReasonCode;
  String accidentCode;
  ENV_TYPE envType;
  uint8_t otaFlag;
  uint8_t eventsID;
  uint8_t isNeutralLogo;
  uint8_t hbInterval;
  uint8_t isAxEnv;
  uint8_t runEnv;
  uint8_t screenLang;
  int clientSts;
  String peerAddress;
} DATA;

typedef struct
{
  volatile uint8_t trigCallRobotFlag;
  volatile uint8_t trigCallRobotEnFlag;
  volatile uint8_t trigCallRobotDisFlag;
  volatile uint8_t trigDecallRobotFlag;
  volatile uint8_t trigDecallRobotEnFlag;
  volatile uint8_t trigDecallRobotDisFlag;
  volatile uint8_t actionFlag;
  volatile uint8_t trigAction;
  volatile uint8_t updateScreenViaBtn;
  volatile uint8_t smartWifiFlag;
  String currAction;
} ACTION_FLAG;

typedef struct _display_vars
{
  uint8_t user_lang;
  uint8_t line_len;
  uint8_t web_display_lang;
} display_vars;

typedef struct
{
  uint8_t ipUpdate;
  uint8_t ipCfgFlag;
  union
  {
    uint8_t ipArr[4];
    uint32_t ip_u32;
  };
} IPUPDATEVAR;

#pragma pack()

extern DATA varData;
extern ACTION_FLAG actionData;
extern display_vars displayVars;
extern IPUPDATEVAR dnsUpdateVar;
extern IPUPDATEVAR staticIpUpdateVar;
extern IPUPDATEVAR staticGwUpdateVar;
extern IPUPDATEVAR staticSbntUpdateVar;

extern uint8_t ap_gw_select_flag;

void init_var_data(void);
bool read_flash_config(void);
void get_run_env(void);
void cfg_wifi_dns(void);
void init_env(void);
bool factory_settings(void);
void check_if_need_factory(void);
bool update_ap_gw(void);
bool update_dns(void);
bool update_wifi_staticip(void);
bool update_wifi(void);
bool update_wpa2_enterprise_wifi(void);
bool update_privet_deploy_vars(void);
bool update_env_type(void);
bool update_web_display_lang(void);

#endif
