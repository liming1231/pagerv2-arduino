// #pragma GCC optimize("-Os")

#include "ax_common.h"

String VERSION;

DATA varData = { 0 };
ACTION_FLAG actionData = { 0 };
display_vars displayVars = { 0 };
IPUPDATEVAR dnsUpdateVar = { 0 };
IPUPDATEVAR staticIpUpdateVar = { 0 };
IPUPDATEVAR staticGwUpdateVar = { 0 };
IPUPDATEVAR staticSbntUpdateVar = { 0 };

// StaticJsonDocument<ERRCODEARR_DOC_LEN> errorCodeArray;
JsonDocument errorCodeArray;

uint8_t ap_gw_select_flag = 0;

void init_var_data(void) {
  varData.callStaId.clear();
  varData.callStaId = mac_s;
  memset(varData.wifiName, 0, sizeof(varData.wifiName));
  memset(varData.wifiProto, 0, sizeof(varData.wifiProto));
  varData.static_ip = 0;
  varData.wpa2_auth_method = WPA2_AUTH_TLS;
  memset(varData.wpa2_identity, 0, sizeof(varData.wpa2_identity));
  memset(varData.wpa2_username, 0, sizeof(varData.wpa2_username));
  memset(varData.wifiPwd, 0, sizeof(varData.wifiPwd));
  memset(varData.priv_addr, 0, sizeof(varData.priv_addr));
  memset(varData.priv_proto, 0, sizeof(varData.priv_proto));
  varData.localProtoType = 0;
  varData.priv_port = 0;
  memset(varData.priv_usr, 0, sizeof(varData.priv_usr));
  memset(varData.priv_pwd, 0, sizeof(varData.priv_pwd));
  varData.gatewayId.clear();
  varData.otaFlag = 0;
  varData.envType = ENV_TYPE::UNCFG;
  varData.isNeutralLogo = 1;
  varData.hbInterval = 10;
  varData.isAxEnv = ALI_ENV;

  actionData.smartWifiFlag = 0;

  displayVars.user_lang = USE_CHINESE_LANG;
  displayVars.line_len = 24;
  displayVars.web_display_lang = USE_CHINESE_LANG;
}

bool read_flash_config(void) {
  nvs_handle handle;
  char gateway_s[24] = { 0 };
  if (nvs_open("storage", NVS_READONLY, &handle) == ESP_OK) {
    size_t length;

    if (ESP_OK != nvs_get_u8(handle, WEB_DIS_LANG, ((uint8_t *)&displayVars.web_display_lang))) {
      displayVars.web_display_lang = USE_CHINESE_LANG;
      if (nvs_set_u8(handle, WEB_DIS_LANG, displayVars.web_display_lang) == ESP_OK) {
        Serial.print("displayVars.web_display_lang:");
        Serial.println(displayVars.web_display_lang);
      }
    }
    if (nvs_get_str(handle, WIFI_NAME, NULL, &length) == ESP_OK && length <= 32) {
      nvs_get_str(handle, WIFI_NAME, varData.wifiName, &length);
      Serial.print("wifiName:");
      Serial.println(varData.wifiName);
    }

    if (nvs_get_str(handle, WIFI_PWD, NULL, &length) == ESP_OK && length <= 64) {
      nvs_get_str(handle, WIFI_PWD, varData.wifiPwd, &length);
      // Serial.print("wifiPwd:");
      // Serial.println(varData.wifiPwd);
    } else {
      memset(varData.wifiPwd, 0, sizeof(varData.wifiPwd));
    }

    if (nvs_get_str(handle, WIFI_PROTO, NULL, &length) == ESP_OK && length <= 24) {
      nvs_get_str(handle, WIFI_PROTO, varData.wifiProto, &length);
      Serial.print("wifiProto:");
      Serial.println(varData.wifiProto);
    }

    if (ESP_OK != nvs_get_u8(handle, WIFI_METHOD, ((uint8_t *)&varData.wpa2_auth_method))) {
      varData.wpa2_auth_method = WPA2_AUTH_TLS;
      Serial.print("wpa2_auth_method:");
      Serial.println(varData.wpa2_auth_method);
    }

    if (nvs_get_str(handle, WIFI_WPA2_ID, NULL, &length) == ESP_OK && length <= 64) {
      nvs_get_str(handle, WIFI_WPA2_ID, varData.wpa2_identity, &length);
      Serial.print("wpa2_identity:");
      Serial.println(varData.wpa2_identity);
    }

    if (nvs_get_str(handle, WIFI_WPA2_USR, NULL, &length) == ESP_OK && length <= 64) {
      nvs_get_str(handle, WIFI_WPA2_USR, varData.wpa2_username, &length);
      Serial.print("wpa2_username:");
      Serial.println(varData.wpa2_username);
    }

    if (nvs_get_str(handle, GATEWAY, NULL, &length) == ESP_OK && length < 24) {
      nvs_get_str(handle, GATEWAY, gateway_s, &length);
      varData.gatewayId = String(gateway_s);
      Serial.print("gatewayId:");
      Serial.println(varData.gatewayId);
    }

    if (ESP_OK != nvs_get_u8(handle, ENV_TYPE_FLASH, ((uint8_t *)&varData.envType))) {
      varData.envType = ENV_TYPE::DEVELOPMENT;
      if (nvs_set_u8(handle, ENV_TYPE_FLASH, varData.envType) == ESP_OK) {
        Serial.print("envType:");
        Serial.println(varData.envType);
      }
    }
    if (ESP_OK != nvs_get_u8(handle, IS_AX_ENV, ((uint8_t *)&varData.isAxEnv))) {
      varData.isAxEnv = ALI_ENV;
      if (nvs_set_u8(handle, IS_AX_ENV, varData.isAxEnv) == ESP_OK) {
        Serial.print("isAxEnv:");
        Serial.println(varData.isAxEnv);
      }
    }
    if (ESP_OK != nvs_get_u8(handle, HEARTBEAT_INTERVAL, ((uint8_t *)&varData.hbInterval))) {
      varData.hbInterval = 10;
      if (nvs_set_u8(handle, ENV_TYPE_FLASH, varData.hbInterval) == ESP_OK) {
        Serial.print("hbInterval:");
        Serial.println(varData.hbInterval);
      }
    }
    if (nvs_get_str(handle, PRIV_PROTO, NULL, &length) == ESP_OK && length <= 8) {
      nvs_get_str(handle, PRIV_PROTO, varData.priv_proto, &length);
      Serial.print("read priv_proto:");
      Serial.println(varData.priv_proto);
    }
    if (nvs_get_str(handle, PRIV_DEPLOY, NULL, &length) == ESP_OK && length < 64) {
      nvs_get_str(handle, PRIV_DEPLOY, varData.priv_addr, &length);
      Serial.print("read priv_addr:");
      Serial.println(varData.priv_addr);
    }
    if (nvs_get_i32(handle, PRIV_DEPLOY_PORT, ((int32_t *)&varData.priv_port)) == ESP_OK) {
      Serial.print("read priv_port:");
      Serial.println(varData.priv_port);
    }
    if (nvs_get_str(handle, PRIV_DEPLOY_USR, NULL, &length) == ESP_OK && length < 16) {
      nvs_get_str(handle, PRIV_DEPLOY_USR, varData.priv_usr, &length);
      Serial.print("read priv_usr:");
      Serial.println(varData.priv_usr);
    }
    if (nvs_get_str(handle, PRIV_DEPLOY_PWD, NULL, &length) == ESP_OK && length < 32) {
      nvs_get_str(handle, PRIV_DEPLOY_PWD, varData.priv_pwd, &length);
      Serial.print("read priv_pwd:");
      Serial.println(varData.priv_pwd);
    }

    if (ESP_OK != nvs_get_u8(handle, DIS_LANG, ((uint8_t *)&displayVars.user_lang))) {
      displayVars.user_lang = LANG_TYPE::USE_CHINESE_LANG;
      displayVars.line_len = 24;
      if (nvs_set_u8(handle, DIS_LANG, displayVars.user_lang) == ESP_OK) {
        Serial.print("dis lang:");
        Serial.println(displayVars.user_lang);
      }
    } else {
      if (displayVars.user_lang == LANG_TYPE::USE_CHINESE_LANG) {
        displayVars.line_len = 24;  // 一行8个中文
      } else if (displayVars.user_lang == LANG_TYPE::USE_ENGLISH_LANG) {
        displayVars.line_len = 16;  // 一行16个字符
      } else if (displayVars.user_lang == LANG_TYPE::USE_JAPANESE_LANG) {
        displayVars.line_len = 30;  // 一行10个日文
      } else {
      }
    }

    if (ESP_OK == nvs_get_u8(handle, DNS_CFG_FLAG, ((uint8_t *)&dnsUpdateVar.ipCfgFlag))) {
      Serial.printf("dnsCfgFlag: %d\r\n", dnsUpdateVar.ipCfgFlag);
      if (dnsUpdateVar.ipCfgFlag == 1) {
        nvs_get_u32(handle, DNS_DATA, &dnsUpdateVar.ip_u32);
      }
    }
    Serial.printf("read dns: %d\r\n", dnsUpdateVar.ip_u32);

    if (ESP_OK != nvs_get_u8(handle, IS_STATICIP, ((uint8_t *)&varData.static_ip))) {
      varData.static_ip = 0;
    } else if (varData.static_ip > 1) {
      varData.static_ip = 0;
    }
    Serial.printf("varData.static_ip: %d\r\n", varData.static_ip);
    if (varData.static_ip == 1) {
      nvs_get_u32(handle, STTC_IP, &staticIpUpdateVar.ip_u32);
      nvs_get_u32(handle, STTC_GW, &staticGwUpdateVar.ip_u32);
      nvs_get_u32(handle, STTC_SNT, &staticSbntUpdateVar.ip_u32);
      nvs_get_u8(handle, DNS_CFG_FLAG, ((uint8_t *)&dnsUpdateVar.ipCfgFlag));
      nvs_get_u32(handle, DNS_DATA, &dnsUpdateVar.ip_u32);
    }

    if (ESP_OK == nvs_get_u8(handle, AP_GATEWAY, ((uint8_t *)&ap_gw_select_flag))) {
      Serial.printf("ap_gw_select_flag: %d\r\n", ap_gw_select_flag);
    }

    delay(1);
    nvs_commit(handle);
    delay(1);
    nvs_close(handle);
    return true;
  } else {
    return false;
  }
}

void get_run_env(void) {
  if (varData.envType == PRIVAT) {
    varData.runEnv = RUN_PRIVAT;
  } else if (varData.envType == DEVELOPMENT) {
    if (varData.isAxEnv == ALI_ENV) {
      varData.runEnv = RUN_DEV_ALI;
    } else if (varData.isAxEnv == AX_BUILD_ENV) {
      varData.runEnv = RUN_DEV_AX_BUILD;
    }
  } else if (varData.envType == PRODUCT) {
    if (varData.isAxEnv == ALI_ENV) {
      varData.runEnv = RUN_PROD_ALI;
    } else if (varData.isAxEnv == AX_BUILD_ENV) {
      varData.runEnv = RUN_PROD_AX_BUILD;
    }
  } else if (varData.envType == FOREIGN) {
    if (varData.isAxEnv == ALI_ENV) {
      varData.runEnv = RUN_FOREIGN_ALI;
    } else if (varData.isAxEnv == AX_BUILD_ENV_GLBL) {
      varData.runEnv = RUN_FOREIGN_AX_BUILD_GLBL;
    } else if (varData.isAxEnv == AX_BUILD_ENV_EU) {
      varData.runEnv = RUN_FOREIGN_AX_BUILD_EU;
    }
  } else if (varData.envType == JAPANESE) {
    if (varData.isAxEnv == ALI_ENV) {
      varData.runEnv = RUN_JAPANESE_ALI;
    } else if (varData.isAxEnv == AX_BUILD_ENV_GLBL) {
      varData.runEnv = RUN_JAPANESE_AX_BUILD_GLBL;
    }
  }
}

void cfg_wifi_dns(void) {
  if (dnsUpdateVar.ipCfgFlag == 0) {
    switch (varData.runEnv) {
      case RUN_PRIVAT:
        {
          dnsUpdateVar.ip_u32 = 0;
          break;
        }
      case RUN_DEV_ALI:
      case RUN_PROD_ALI:
      case RUN_FOREIGN_ALI:
      case RUN_JAPANESE_ALI:
        {
          dnsUpdateVar.ipArr[0] = 223;
          dnsUpdateVar.ipArr[1] = 5;
          dnsUpdateVar.ipArr[2] = 5;
          dnsUpdateVar.ipArr[3] = 5;
          break;
        }
      case RUN_DEV_AX_BUILD:
      case RUN_PROD_AX_BUILD:
      case RUN_FOREIGN_AX_BUILD_GLBL:
      case RUN_FOREIGN_AX_BUILD_EU:
      case RUN_JAPANESE_AX_BUILD_GLBL:
        {
          dnsUpdateVar.ipArr[0] = 8;
          dnsUpdateVar.ipArr[1] = 8;
          dnsUpdateVar.ipArr[2] = 8;
          dnsUpdateVar.ipArr[3] = 8;
          break;
        }
      default:
        {
          dnsUpdateVar.ipArr[0] = 8;
          dnsUpdateVar.ipArr[1] = 8;
          dnsUpdateVar.ipArr[2] = 8;
          dnsUpdateVar.ipArr[3] = 8;
          break;
        }
    }
  }
  // if (dnsUpdateVar.ipCfgFlag == 1)
  // {
  //     if (varData.envType == PRIVAT)
  //     {
  //         dnsUpdateVar.ip_u32 = 0;
  //     }
  // }
  Serial.printf("dns:%d.%d.%d.%d\r\n", dnsUpdateVar.ipArr[0], dnsUpdateVar.ipArr[1], dnsUpdateVar.ipArr[2], dnsUpdateVar.ipArr[3]);
}

/*
 * 指定运行环境和版本号
 */
void init_env(void) {
  switch (varData.envType) {
    case ENV_TYPE::PRIVAT:
      {
        if (displayVars.user_lang == LANG_TYPE::USE_CHINESE_LANG) {
          VERSION = VERSION_NUM VERSION_CN_LOC;
        } else if (displayVars.user_lang == LANG_TYPE::USE_ENGLISH_LANG) {
          VERSION = VERSION_NUM VERSION_EN_LOC;
        } else if (displayVars.user_lang == LANG_TYPE::USE_JAPANESE_LANG) {
          VERSION = VERSION_NUM VERSION_JP_LOC;
        } else {
          VERSION = VERSION_NUM VERSION_CN_LOC;
        }

        break;
      }
    case ENV_TYPE::DEVELOPMENT:
      {
        if (displayVars.user_lang == LANG_TYPE::USE_CHINESE_LANG) {
          VERSION = VERSION_NUM VERSION_CN_DEV;
        } else if (displayVars.user_lang == LANG_TYPE::USE_ENGLISH_LANG) {
          VERSION = VERSION_NUM VERSION_EN_DEV;
        } else if (displayVars.user_lang == LANG_TYPE::USE_JAPANESE_LANG) {
          VERSION = VERSION_NUM VERSION_JP_DEV;
        } else {
          VERSION = VERSION_NUM VERSION_CN_DEV;
        }

        break;
      }
    case ENV_TYPE::PRODUCT:
      {
        if (displayVars.user_lang == LANG_TYPE::USE_CHINESE_LANG) {
          VERSION = VERSION_NUM VERSION_CN;
        } else if (displayVars.user_lang == LANG_TYPE::USE_ENGLISH_LANG) {
          VERSION = VERSION_NUM VERSION_EN;
        } else if (displayVars.user_lang == LANG_TYPE::USE_JAPANESE_LANG) {
          VERSION = VERSION_NUM VERSION_JP;
        } else {
          VERSION = VERSION_NUM VERSION_CN;
        }

        break;
      }
    case ENV_TYPE::FOREIGN:
      {
        if (displayVars.user_lang == LANG_TYPE::USE_CHINESE_LANG) {
          VERSION = VERSION_NUM VERSION_CN;
        } else if (displayVars.user_lang == LANG_TYPE::USE_ENGLISH_LANG) {
          VERSION = VERSION_NUM VERSION_EN_INTERNATIONAL;
        } else if (displayVars.user_lang == LANG_TYPE::USE_JAPANESE_LANG) {
          VERSION = VERSION_NUM VERSION_JAPANESE;
        } else {
          VERSION = VERSION_NUM VERSION_CN_INTERNATIONAL;
        }

        break;
      }
    case ENV_TYPE::JAPANESE:
      {
        VERSION = VERSION_NUM VERSION_JAPANESE;
        break;
      }

    default:
      {
        if (displayVars.user_lang == LANG_TYPE::USE_CHINESE_LANG) {
          VERSION = VERSION_NUM VERSION_CN;
        } else if (displayVars.user_lang == LANG_TYPE::USE_ENGLISH_LANG) {
          VERSION = VERSION_NUM VERSION_EN;
        } else if (displayVars.user_lang == LANG_TYPE::USE_JAPANESE_LANG) {
          VERSION = VERSION_NUM VERSION_JP;
        } else {
          VERSION = VERSION_NUM VERSION_CN;
        }
        break;
      }
  }
}

// bool factory_settings(void) {
//   nvs_handle handle;
//   if (nvs_open("storage", NVS_READWRITE, &handle) == ESP_OK) {
//     nvs_erase_key(handle, WIFI_NAME);
//     delay(1);
//     nvs_erase_key(handle, WIFI_PWD);
//     delay(1);
//     nvs_erase_key(handle, WIFI_PROTO);
//     delay(1);
//     nvs_erase_key(handle, WIFI_METHOD);
//     delay(1);
//     nvs_erase_key(handle, WIFI_WPA2_ID);
//     delay(1);
//     nvs_erase_key(handle, WIFI_WPA2_USR);
//     delay(1);
//     nvs_erase_key(handle, GATEWAY);
//     delay(1);
//     nvs_erase_key(handle, ENV_TYPE_FLASH);
//     delay(1);
//     nvs_erase_key(handle, IS_AX_ENV);
//     delay(1);
//     nvs_erase_key(handle, PRIV_PROTO);
//     delay(1);
//     nvs_erase_key(handle, PRIV_DEPLOY);
//     delay(1);
//     nvs_erase_key(handle, PRIV_DEPLOY_PORT);
//     delay(1);
//     nvs_erase_key(handle, PRIV_DEPLOY_USR);
//     delay(1);
//     nvs_erase_key(handle, PRIV_DEPLOY_PWD);
//     delay(1);
//     nvs_erase_key(handle, DIS_LANG);
//     delay(1);
//     nvs_erase_key(handle, WEB_DIS_LANG);
//     delay(1);
//     nvs_erase_key(handle, HEARTBEAT_INTERVAL);
//     delay(1);
//     nvs_erase_key(handle, DNS_DATA);
//     delay(1);
//     nvs_erase_key(handle, DNS_CFG_FLAG);
//     delay(1);
//     nvs_erase_key(handle, AP_GATEWAY);
//     delay(1);
//     nvs_erase_key(handle, IS_STATICIP);
//     delay(1);
//     nvs_erase_key(handle, STTC_IP);
//     delay(1);
//     nvs_erase_key(handle, STTC_GW);
//     delay(1);
//     nvs_erase_key(handle, STTC_SNT);
//     delay(1);
//     nvs_commit(handle);
//     delay(1);
//     nvs_close(handle);
//     return true;
//   } else {
//     return false;
//   }
// }

void check_if_need_factory(void) {
  unsigned long startupTimestep = millis();
  unsigned long flushTimestep = startupTimestep;
  while (true) {
    flushTimestep = millis();
    if (flushTimestep - startupTimestep <= 2000) {
      if (digitalRead(DECALL_BUTTON) == LOW) {
        while (true) {
          flushTimestep = millis();
          if (digitalRead(DECALL_BUTTON) == HIGH) {
            break;
          } else if (flushTimestep - startupTimestep >= 2000) {
            // factory_settings();
            yield();
            delay(1000);
            yield();
            ESP.restart();
          }
        }
      }
    } else {
      break;
    }
  }
}
