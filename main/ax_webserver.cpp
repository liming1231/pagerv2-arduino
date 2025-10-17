#include "ax_common.h"

const char *AP_SSID_HEADER = "pager_";
const char *AP_PASS = "AutoXing";
IPAddress LocalIP(10, 25, 25, 1);    // 设置本地IP地址
IPAddress GateWay(10, 25, 25, 1);    // 设置网关
IPAddress SubNet(255, 255, 255, 0);  // 设置子网掩码

cfg_wifi cfgWifi;
web_infos webInfos;


WiFiClient espClient;
WiFiClientSecure ethClientSSL;
PubSubClient client(espClient);

// AsyncWebServer server_test(8088); // 创建网络及部署服务器对象，指定监听端口8088
// AsyncWebServer server_cfg(8088);    // 创建网络及部署服务器对象，指定监听端口8088
// AsyncWebServer server_update(8089); // 创建版本查询及OTA服务器对象，指定监听端口8089

WebServer server_cfg(8088);
// const char *host = "esp32";

String get_deploy_env_info();
String get_mqtt_server_info();

String get_deploy_env_info_n() {
  String env_m;
  switch (varData.envType) {
    case UNCFG:
      {
        if (displayVars.web_display_lang == USE_CHINESE_LANG)
          env_m = "未配置";
        else
          env_m = "unconfig";
        break;
      }
    case PRIVAT:
      {
        if (displayVars.web_display_lang == USE_CHINESE_LANG)
          env_m = "私有化环境";
        else
          env_m = "PRIVAT";
        break;
      }
    case DEVELOPMENT:
      {
        if (displayVars.web_display_lang == USE_CHINESE_LANG)
          env_m = "开发环境";
        else
          env_m = "DEVELOPMENT";
        break;
      }
    case PRODUCT:
      {
        if (displayVars.web_display_lang == USE_CHINESE_LANG)
          env_m = "生产环境";
        else
          env_m = "PRODUCT";
        break;
      }
    case FOREIGN:
      {
        if (displayVars.web_display_lang == USE_CHINESE_LANG)
          env_m = "海外环境";
        else
          env_m = "FOREIGN";
        break;
      }
    case JAPANESE:
      {
        if (displayVars.web_display_lang == USE_CHINESE_LANG)
          env_m = "日本环境";
        else
          env_m = "JAPANESE";
        break;
      }
    default:
      break;
  }
  return env_m;
}

String get_mqtt_server_info_n() {
  String env_m;
  if (varData.envType == ENV_TYPE::PRIVAT) {
    env_m = "PRIVAT";
  } else {
    switch (varData.isAxEnv) {
      case ALI_ENV:
        {
          env_m = "ALIYUN";
          break;
        }
      case AX_BUILD_ENV:
        {
          env_m = "AX_CN";
          break;
        }
      case AX_BUILD_ENV_GLBL:
        {
          env_m = "AX_GLOBAL";
          break;
        }
      case AX_BUILD_ENV_EU:
        {
          env_m = "AX_EUROPE";
          break;
        }
      default:
        {
          env_m = "UNKNOWN";
          break;
        }
    }
  }
  return env_m;
}

JsonDocument web_sts;
void send_infos(WebServer *request) {
  char myDoc[measureJson(web_sts) + 1];
  web_sts.clear();
  web_sts["sn"] = varData.callStaId;
  web_sts["version"] = VERSION;
  if (displayVars.web_display_lang == USE_CHINESE_LANG)
    web_sts["web_lang"] = "中文";
  else
    web_sts["web_lang"] = "ENGLISH";
  if (displayVars.user_lang == USE_CHINESE_LANG)
    web_sts["deploy_lang"] = "中文";
  else if (displayVars.user_lang == USE_JAPANESE_LANG)
    web_sts["deploy_lang"] = "JAPANESE";
  else
    web_sts["deploy_lang"] = "ENGLISH";
  web_sts["ap_gw"] = GateWay.toString();
  web_sts["SSID"] = varData.wifiName;
  web_sts["Password"] = varData.wifiPwd;
  web_sts["Static_ip"] = varData.static_ip == 0 ? false : true;
  if (web_sts["Static_ip"] == false) {
    web_sts["ip"] = WiFi.localIP().toString();
  } else {
    web_sts["Static_ip_addr"] = WiFi.localIP().toString();
    web_sts["Static_gateway"] = WiFi.gatewayIP().toString();
    web_sts["Static_subnet"] = WiFi.subnetMask().toString();
  }
  web_sts["dns_server"] = WiFi.dnsIP().toString();
  web_sts["rssi"] = WiFi.RSSI();
  web_sts["bssid"] = WiFi.BSSIDstr();

  switch (varData.runEnv) {
    case RUN_PRIVAT:
      {
        web_sts["deploy_env"] = "PRIVATE";
        break;
      }
    case RUN_DEV_ALI:
      {
        web_sts["deploy_env"] = "DEVELOPMENT_ALI";
        break;
      }
    case RUN_DEV_AX_BUILD:
      {
        web_sts["deploy_env"] = "DEVELOPMENT_AX";
        break;
      }
    case RUN_PROD_ALI:
      {
        web_sts["deploy_env"] = "PRODUCT_ALI";
        break;
      }
    case RUN_PROD_AX_BUILD:
      {
        web_sts["deploy_env"] = "PRODUCT_AX";
        break;
      }
    case RUN_FOREIGN_ALI:
      {
        web_sts["deploy_env"] = "FOREIGN_ALI";
        break;
      }
    case RUN_FOREIGN_AX_BUILD_GLBL:
      {
        web_sts["deploy_env"] = "FOREIGN_AX";
        break;
      }
    case RUN_FOREIGN_AX_BUILD_EU:
      {
        web_sts["deploy_env"] = "EU_AX";
        break;
      }
    case RUN_JAPANESE_ALI:
    case RUN_JAPANESE_AX_BUILD_GLBL:
      {
        web_sts["deploy_env"] = "JAPANESE";
        break;
      }
    default:
      break;
  }
  web_sts["deploy_type"] = get_deploy_env_info_n();
  web_sts["mqtt_type"] = get_mqtt_server_info_n();
  web_sts["mqtt_online"] = client.connected();

  if (client.connected() != 1) {
    web_sts["mqtt-rc"] = varData.clientSts;
  }
  web_sts["mqtt_srv_ip"] = varData.peerAddress;

  if (strncmp(varData.wifiProto, "WPA2_ENTERPRISE", 15) == 0) {
    web_sts["wpa2EnterpriseAuthMethod"] = cfgWifi.method;
    web_sts["wpa2EnterpriseAuthIdentity"] = cfgWifi.wpa2_identity;
    web_sts["wpa2EnterpriseAuthUsername"] = cfgWifi.wpa2_username;
  }
  if (varData.envType == PRIVAT) {
    web_sts["dagreement_type"] = varData.priv_proto;
    web_sts["priv_addr"] = varData.priv_addr;
    web_sts["priv_port"] = varData.priv_port;
    web_sts["priv_usr"] = varData.priv_usr;
    web_sts["priv_pwd"] = varData.priv_pwd;
  }

  serializeJson(web_sts, myDoc, measureJson(web_sts) + 1);
  request->send(200, "application/json", myDoc);
}

void send_infos_bak(WebServer *request) {
  String ret = "{";
  ret += webInfos.sn + ",";
  ret += webInfos.version + ",";
  ret += webInfos.web_lang + ",";
  ret += webInfos.deploy_lang + ",";
  ret += KEY_1("ap_gw") + VALUE_DEF(GateWay.toString()) + ",";
  ret += webInfos.ssid + ",";
  ret += webInfos.password + ",";
  ret += KEY_1("ip") + VALUE_DEF(WiFi.localIP().toString()) + ",";
  ret += KEY_1("rssi") + VALUE_DEF(WiFi.RSSI()) + ",";
  ret += KEY_1("bssid") + VALUE_DEF(WiFi.BSSIDstr()) + ",";
  switch (varData.runEnv) {
    case RUN_PRIVAT:
      {
        ret += KEY_1("deploy_env") + VALUE_DEF("PRIVATE") + ",";
        break;
      }
    case RUN_DEV_ALI:
      {
        ret += KEY_1("deploy_env") + VALUE_DEF("DEVELOPMENT_ALI") + ",";
        break;
      }
    case RUN_DEV_AX_BUILD:
      {
        ret += KEY_1("deploy_env") + VALUE_DEF("DEVELOPMENT_AX") + ",";
        break;
      }
    case RUN_PROD_ALI:
      {
        ret += KEY_1("deploy_env") + VALUE_DEF("PRODUCT_ALI") + ",";
        break;
      }
    case RUN_PROD_AX_BUILD:
      {
        ret += KEY_1("deploy_env") + VALUE_DEF("PRODUCT_AX") + ",";
        break;
      }
    case RUN_FOREIGN_ALI:
      {
        ret += KEY_1("deploy_env") + VALUE_DEF("FOREIGN_ALI") + ",";
        break;
      }
    case RUN_FOREIGN_AX_BUILD_GLBL:
      {
        ret += KEY_1("deploy_env") + VALUE_DEF("FOREIGN_AX") + ",";
        break;
      }
    case RUN_FOREIGN_AX_BUILD_EU:
      {
        ret += KEY_1("deploy_env") + VALUE_DEF("EU_AX") + ",";
        break;
      }
    case RUN_JAPANESE_ALI:
    case RUN_JAPANESE_AX_BUILD_GLBL:
      {
        ret += KEY_1("deploy_env") + VALUE_DEF("JAPANESE") + ",";
        break;
      }
    default:
      break;
  }
  ret += webInfos.deploy_env + ",";
  ret += webInfos.mqtt_server + ",";
  ret += webInfos.mqtt_online + client.connected() + ",";
  if (client.connected() != 1) {
    ret += KEY_1("mqtt-rc") + VALUE_DEF(varData.clientSts) + String(",");
  }

  ret += webInfos.mqtt_server_ip + VALUE_DEF(varData.peerAddress) + ",";

  if (strncmp(varData.wifiProto, "WPA2_ENTERPRISE", 15) == 0) {
    ret += webInfos.wpa2EnterpriseAuthMethod + ",";
    ret += webInfos.wpa2EnterpriseAuthIdentity + ",";
    ret += webInfos.wpa2EnterpriseAuthUsername + ",";
  }
  if (varData.envType == PRIVAT) {
    ret += webInfos.priv_proto + ",";
    ret += webInfos.priv_addr + ",";
    ret += webInfos.priv_port + ",";
    ret += webInfos.priv_usr + ",";
    ret += webInfos.priv_pwd + ",";
  }
  ret += webInfos.dns + VALUE_DEF(WiFi.dnsIP().toString());
  ret += "}";

  // AsyncWebServerResponse *response = request->beginResponse(200, "application/json", ret);
  request->send(200, "application/json", ret);
}

void parse_ap_gw(WebServer *request) {
  if (!request->hasArg("ap_gw")) {
    Serial.println("AP GW Setting Failed, No Matched ap_gw.");
    request->send(200, "application/json", "\"{res\":500}");
  } else {
    uint8_t updateApGatewayFlag = 0;
    String ap_gw_type = request->arg("ap_gw");
    Serial.printf("ap_gw_type:%s\r\n", ap_gw_type);
    if (ap_gw_type == AP_GW_1) {
      Serial.println("ap_gw: " + String(AP_GW_1));
      ap_gw_select_flag = 0;
      updateApGatewayFlag = 1;
    } else if (ap_gw_type == AP_GW_2) {
      Serial.println("ap_gw: " + String(AP_GW_2));
      ap_gw_select_flag = 1;
      updateApGatewayFlag = 1;
    } else {
      updateApGatewayFlag = 0;
    }

    if (updateApGatewayFlag == 1) {
      if (update_ap_gw()) {
        request->send(200, "application/json", "{\"res\":200}");
      } else {
        request->send(200, "application/json", "{\"res\":500}");
      }
      delay(500);
    } else {
      request->send(200, "application/json", "{\"res\":500}");
    }
  }
}

bool is_static_ip_addr_valid() {
  uint8_t index;
  uint8_t ipxLen;
  uint8_t validFlag = 1;
  uint32_t static_ipaddr_t;
  char static_ipaddr_m[20] = { 0 };
  memcpy(static_ipaddr_m, cfgWifi.Static_ip_addr.c_str(), cfgWifi.Static_ip_addr.length());
  char *token = strtok(static_ipaddr_m, ".");
  while (token != NULL) {
    ipxLen = strlen(token);
    if (ipxLen > 3) {
      validFlag = 0;
      break;
    } else {
      static_ipaddr_t = atoi(token);
      if (index >= 4) {
        validFlag = 0;
        break;
      } else if (static_ipaddr_t <= 255) {
        staticIpUpdateVar.ipArr[index] = (uint8_t)static_ipaddr_t;
        index++;
        token = strtok(NULL, ".");
      } else {
        validFlag = 0;
        break;
      }
    }
  }

  if ((index == 4) && (validFlag == 1)) {
    if (staticIpUpdateVar.ip_u32 == 0) {
      staticIpUpdateVar.ipCfgFlag = 0;
      staticIpUpdateVar.ipUpdate = 0;
      return false;
    } else {
      staticIpUpdateVar.ipCfgFlag = 1;
      staticIpUpdateVar.ipUpdate = 1;
      return true;
    }
  } else {
    staticIpUpdateVar.ipUpdate = 0;
    return false;
  }
}

bool is_static_gateway_valid() {
  uint8_t index;
  uint8_t ipxLen;
  uint8_t validFlag = 1;
  uint32_t static_gateway_t;
  char static_gateawy_m[20] = { 0 };
  memcpy(static_gateawy_m, cfgWifi.Static_gateway.c_str(), cfgWifi.Static_gateway.length());
  char *token = strtok(static_gateawy_m, ".");
  while (token != NULL) {
    ipxLen = strlen(token);
    if (ipxLen > 3) {
      validFlag = 0;
      break;
    } else {
      static_gateway_t = atoi(token);
      if (index >= 4) {
        validFlag = 0;
        break;
      } else if (static_gateway_t <= 255) {
        staticGwUpdateVar.ipArr[index] = (uint8_t)static_gateway_t;
        index++;
        token = strtok(NULL, ".");
      } else {
        validFlag = 0;
        break;
      }
    }
  }

  if ((index == 4) && (validFlag == 1)) {
    if (staticGwUpdateVar.ip_u32 == 0) {
      staticGwUpdateVar.ipCfgFlag = 0;
      staticGwUpdateVar.ipUpdate = 0;
      return false;
    } else {
      staticGwUpdateVar.ipCfgFlag = 1;
      staticGwUpdateVar.ipUpdate = 1;
      return true;
    }
  } else {
    staticGwUpdateVar.ipUpdate = 0;
    return false;
  }
}

bool is_static_subnet_valid() {
  uint8_t index;
  uint8_t ipxLen;
  uint8_t validFlag = 1;
  uint32_t static_subnet_t;
  char static_subnet_m[20] = { 0 };
  memcpy(static_subnet_m, cfgWifi.Static_subnet.c_str(), cfgWifi.Static_subnet.length());
  char *token = strtok(static_subnet_m, ".");
  while (token != NULL) {
    ipxLen = strlen(token);
    if (ipxLen > 3) {
      validFlag = 0;
      break;
    } else {
      static_subnet_t = atoi(token);
      if (index >= 4) {
        validFlag = 0;
        break;
      } else if (static_subnet_t <= 255) {
        staticSbntUpdateVar.ipArr[index] = (uint8_t)static_subnet_t;
        index++;
        token = strtok(NULL, ".");
      } else {
        validFlag = 0;
        break;
      }
    }
  }

  if ((index == 4) && (validFlag == 1)) {
    if (staticSbntUpdateVar.ip_u32 == 0) {
      staticSbntUpdateVar.ipCfgFlag = 0;
      staticSbntUpdateVar.ipUpdate = 0;
      return false;
    } else {
      staticSbntUpdateVar.ipCfgFlag = 1;
      staticSbntUpdateVar.ipUpdate = 1;
      return true;
    }
  } else {
    staticSbntUpdateVar.ipUpdate = 0;
    return false;
  }
}

bool is_static_dns_valid() {
  uint8_t index;
  uint8_t ipxLen;
  uint8_t validFlag = 1;
  uint32_t static_dns_t;
  char static_dns_m[20] = { 0 };
  memcpy(static_dns_m, cfgWifi.dns.c_str(), cfgWifi.dns.length());
  char *token = strtok(static_dns_m, ".");
  while (token != NULL) {
    ipxLen = strlen(token);
    if (ipxLen > 3) {
      validFlag = 0;
      break;
    } else {
      static_dns_t = atoi(token);
      if (index >= 4) {
        validFlag = 0;
        break;
      } else if (static_dns_t <= 255) {
        dnsUpdateVar.ipArr[index] = (uint8_t)static_dns_t;
        index++;
        token = strtok(NULL, ".");
      } else {
        validFlag = 0;
        break;
      }
    }
  }

  if ((index == 4) && (validFlag == 1)) {
    if (dnsUpdateVar.ip_u32 == 0) {
      dnsUpdateVar.ipCfgFlag = 0;
    } else {
      dnsUpdateVar.ipCfgFlag = 1;
    }
    dnsUpdateVar.ipUpdate = 1;
    return true;
  } else {
    dnsUpdateVar.ipUpdate = 0;
    return false;
  }
}

bool is_ip_lenth_avlid(String ip_str) {
  if ((ip_str.length() > 15) || (ip_str.length() < 7)) {
    return false;
  }
  return true;
}

bool update_dns_w() {
  char dns_m[20] = { 0 };
  uint8_t index;
  uint32_t dns_t;
  uint8_t validFlag = 1;
  uint8_t ipxLen;
  Serial.printf("dns len: %d\r\n", cfgWifi.dns.length());

  if (!is_ip_lenth_avlid(cfgWifi.dns)) {
    return false;
  } else {
    memcpy(dns_m, cfgWifi.dns.c_str(), cfgWifi.dns.length());
    char *token = strtok(dns_m, ".");
    while (token != NULL) {
      ipxLen = strlen(token);
      Serial.printf("ipxLen: %d\r\n", ipxLen);
      Serial.printf("token: %s\r\n", token);
      if (ipxLen > 3) {
        validFlag = 0;
        break;
      } else {
        dns_t = atoi(token);
        Serial.printf("dns_t: %d\r\n", dns_t);
        if (index >= 4) {
          validFlag = 0;
          break;
        } else if (dns_t <= 255) {
          dnsUpdateVar.ipArr[index] = (uint8_t)dns_t;
          index++;
          token = strtok(NULL, ".");
        } else {
          validFlag = 0;
          break;
        }
      }
    }
    Serial.printf("index: %d\t validFlag: %d\r\n", index, validFlag);
    if ((index == 4) && (validFlag == 1)) {
      Serial.printf("dnsArr[0]: %d\r\n", dnsUpdateVar.ipArr[0]);
      Serial.printf("dnsArr[1]: %d\r\n", dnsUpdateVar.ipArr[1]);
      Serial.printf("dnsArr[2]: %d\r\n", dnsUpdateVar.ipArr[2]);
      Serial.printf("dnsArr[3]: %d\r\n", dnsUpdateVar.ipArr[3]);
      dnsUpdateVar.ipUpdate = 1;
      if (dnsUpdateVar.ip_u32 == 0) {
        dnsUpdateVar.ipCfgFlag = 0;
      } else {
        dnsUpdateVar.ipCfgFlag = 1;
      }
      if (update_dns())
        return true;
      else
        return false;
    } else {
      return false;
    }
  }
}

bool update_static_ip() {
  char static_ipaddr_m[20] = { 0 };
  uint8_t index;
  uint32_t static_ipaddr_t;
  uint8_t validFlag = 1;
  uint8_t ipxLen;

  if (cfgWifi.Static_ip) {
    if (!is_ip_lenth_avlid(cfgWifi.Static_ip_addr)) {
      return false;
    } else if (!is_ip_lenth_avlid(cfgWifi.Static_gateway)) {
      return false;
    } else if (!is_ip_lenth_avlid(cfgWifi.Static_subnet)) {
      return false;
    } else if (!is_ip_lenth_avlid(cfgWifi.dns)) {
      return false;
    } else {
      if (is_static_ip_addr_valid() && is_static_gateway_valid() && is_static_subnet_valid() && is_static_dns_valid()) {
        Serial.printf("updateWifi_staticIP\r\n");
        return update_wifi_staticip();
      } else {
        return false;
      }
    }
  } else {
    return update_dns_w();
  }
}

void parse_wifi_cfg(WebServer *request) {
  uint8_t res_sts = 0;
  uint8_t updateApGatewayFlag = 0;

  cfgWifi.input_SSID = request->arg("SSID");
  cfgWifi.input_Password = request->arg("Password");
  cfgWifi.proto = request->arg("encryption");
  cfgWifi.method = request->arg("protocol");
  cfgWifi.wpa2_identity = request->arg("wpa2_identity");
  cfgWifi.wpa2_username = request->arg("wpa2_username");
  cfgWifi.Static_ip = (request->arg("Static_ip") == "true") ? true : false;
  cfgWifi.Static_ip_addr = request->arg("Static_ip_addr");
  cfgWifi.Static_gateway = request->arg("Static_gateway");
  cfgWifi.Static_subnet = request->arg("Static_subnet");
  cfgWifi.dns = request->arg("dns_server");

  if ((!cfgWifi.input_SSID.isEmpty()) && (!cfgWifi.input_Password.isEmpty())) {
    if (cfgWifi.method.isEmpty() || cfgWifi.wpa2_identity.isEmpty() || cfgWifi.wpa2_username.isEmpty()) {
      strcpy(varData.wifiName, cfgWifi.input_SSID.c_str());
      if (cfgWifi.input_Password == "null") {
        memset(varData.wifiPwd, 0, sizeof(varData.wifiPwd));
      } else {
        strcpy(varData.wifiPwd, cfgWifi.input_Password.c_str());
      }
      Serial.printf("varData.wifiName:%s\r\n", varData.wifiName);
      Serial.printf("varData.wifiPwd:%s\r\n", varData.wifiPwd);
      if (update_wifi()) {
        Serial.println("set wifi to flash succ");
        res_sts |= 1;
      }
    } else {
      strcpy(varData.wifiName, cfgWifi.input_SSID.c_str());
      strcpy(varData.wifiProto, cfgWifi.proto.c_str());
      if (cfgWifi.method == "WPA2_AUTH_TLS") {
        varData.wpa2_auth_method = WPA2_AUTH_TLS;
      } else if (cfgWifi.method == "WPA2_AUTH_PEAP") {
        varData.wpa2_auth_method = WPA2_AUTH_PEAP;
      } else if (cfgWifi.method == "WPA2_AUTH_TTLS") {
        varData.wpa2_auth_method = WPA2_AUTH_TTLS;
      }
      strcpy(varData.wpa2_identity, cfgWifi.wpa2_identity.c_str());
      strcpy(varData.wpa2_username, cfgWifi.wpa2_username.c_str());
      if (cfgWifi.input_Password == "null") {
        memset(varData.wifiPwd, 0, sizeof(varData.wifiPwd));
      } else {
        strcpy(varData.wifiPwd, cfgWifi.input_Password.c_str());
      }
      if (update_wpa2_enterprise_wifi())
        res_sts |= 1;
    }

    if (update_static_ip()) {
      res_sts |= (1 << 1);
    } else {
      ;
    }
  }

  if (res_sts == 3) {
    varData.deslayFlag = WIFICFGSUCC;
    request->send(200, "application/json", "{\"res\":200}");
  } else {
    request->send(200, "application/json", "{\"res\":500}");
  }
}

void parse_private_deploy(WebServer *request) {
  if ((!request->hasArg("deploy_lang")) || (!request->hasArg("deploy_env")) || (!request->hasArg("dagreement_type"))) {
    request->send(200, "application/json", "{\"res\":500}");
  } else {
    cfgWifi.dagreement_type = request->arg("dagreement_type");
    cfgWifi.deploy_lang = request->arg("deploy_lang");
    cfgWifi.deploy_env = request->arg("deploy_env");
    if ((cfgWifi.deploy_lang.isEmpty()) || (cfgWifi.deploy_env.isEmpty())) {
      request->send(200, "application/json", "{\"res\":500}");
    } else {
      Serial.printf("dagreement type: %s\r\n", cfgWifi.dagreement_type);
      Serial.printf("new lang: %s\r\n", cfgWifi.deploy_lang);
      Serial.printf("new env: %s\r\n", cfgWifi.deploy_env);
      if ((cfgWifi.deploy_lang == "CHINESE") || (cfgWifi.deploy_lang == "中文")) {
        displayVars.user_lang = USE_CHINESE_LANG;
        displayVars.line_len = 24;
      } else if ((cfgWifi.deploy_lang == "ENGLISH") || (cfgWifi.deploy_lang == "英文")) {
        displayVars.user_lang = USE_ENGLISH_LANG;
        displayVars.line_len = 16;
      } else if ((cfgWifi.deploy_lang == "JAPANESE") || (cfgWifi.deploy_lang == "日文")) {
        displayVars.user_lang = USE_JAPANESE_LANG;
        displayVars.line_len = 30;
      } else {
        displayVars.user_lang = USE_CHINESE_LANG;
        displayVars.line_len = 24;
      }
      init_display_language();

      if (cfgWifi.deploy_env == "PRIVATE") {
        varData.envType = PRIVAT;
        varData.runEnv = RUN_PRIVAT;
      } else if (cfgWifi.deploy_env == "DEVELOPMENT_ALI") {
        varData.envType = DEVELOPMENT;
        varData.isAxEnv = ALI_ENV;
        varData.runEnv = RUN_DEV_ALI;
      } else if (cfgWifi.deploy_env == "DEVELOPMENT_AX") {
        varData.envType = DEVELOPMENT;
        varData.isAxEnv = AX_BUILD_ENV;
        varData.runEnv = RUN_DEV_AX_BUILD;
      } else if (cfgWifi.deploy_env == "PRODUCT_ALI") {
        varData.envType = PRODUCT;
        varData.isAxEnv = ALI_ENV;
        varData.runEnv = RUN_PROD_ALI;
      } else if (cfgWifi.deploy_env == "PRODUCT_AX") {
        varData.envType = PRODUCT;
        varData.isAxEnv = AX_BUILD_ENV;
        varData.runEnv = RUN_PROD_AX_BUILD;
      } else if (cfgWifi.deploy_env == "FOREIGN_ALI") {
        varData.envType = FOREIGN;
        varData.isAxEnv = ALI_ENV;
        varData.runEnv = RUN_FOREIGN_ALI;
      } else if (cfgWifi.deploy_env == "FOREIGN_AX") {
        varData.envType = FOREIGN;
        varData.isAxEnv = AX_BUILD_ENV_GLBL;
        varData.runEnv = RUN_FOREIGN_AX_BUILD_GLBL;
      } else if (cfgWifi.deploy_env == "EU_AX") {
        varData.envType = FOREIGN;
        varData.isAxEnv = AX_BUILD_ENV_EU;
        varData.runEnv = RUN_FOREIGN_AX_BUILD_EU;
      } else if (cfgWifi.deploy_env == "JAPANESE") {
        varData.envType = JAPANESE;
        varData.isAxEnv = ALI_ENV;
        varData.runEnv = RUN_JAPANESE_ALI;
      }

      if (varData.envType == PRIVAT) {
        if (cfgWifi.dagreement_type == "tcp") {
          memcpy(varData.priv_proto, "tcp", 4);
        } else if (cfgWifi.dagreement_type == "tls") {
          memcpy(varData.priv_proto, "tls", 4);
        } else {
          memcpy(varData.priv_proto, "tcp", 4);
        }
        if (request->hasArg("priv_addr") && request->hasArg("priv_port") && request->hasArg("priv_usr") && request->hasArg("priv_pwd")) {
          cfgWifi.priv_serv_addr = request->arg("priv_addr");
          cfgWifi.priv_serv_port = request->arg("priv_port").toInt();
          cfgWifi.mqtt_priv_usr = request->arg("priv_usr");
          cfgWifi.mqtt_priv_pwd = request->arg("priv_pwd");
          Serial.print("priv_addr:");
          Serial.println(cfgWifi.priv_serv_addr);
          Serial.printf("priv_port:%d\r\n", cfgWifi.priv_serv_port);
          Serial.print("priv_usr:");
          Serial.println(cfgWifi.mqtt_priv_usr);
          Serial.print("priv_pwd:");
          Serial.println(cfgWifi.mqtt_priv_pwd);
          if ((cfgWifi.priv_serv_addr.length() > 7) && (cfgWifi.priv_serv_addr.length() < 64) && (cfgWifi.priv_serv_port >= 1000) && (cfgWifi.priv_serv_port <= 999999) && (cfgWifi.mqtt_priv_usr.length() > 0) && (cfgWifi.mqtt_priv_usr.length() < 16) && (cfgWifi.mqtt_priv_pwd.length() > 0) && (cfgWifi.mqtt_priv_pwd.length() < 32)) {
            request->send(200, "application/json", "{\"res\":200}");
            strcpy(varData.priv_addr, cfgWifi.priv_serv_addr.c_str());
            varData.priv_port = cfgWifi.priv_serv_port;
            strcpy(varData.priv_usr, cfgWifi.mqtt_priv_usr.c_str());
            strcpy(varData.priv_pwd, cfgWifi.mqtt_priv_pwd.c_str());
            update_privet_deploy_vars();
            varData.deslayFlag = DEPLOY_ENV_SUCC;
            delay(1000);
            // esp_restart();
          } else {
            varData.deslayFlag = DEPLOY_ENV_FAILED;
            request->send(200, "application/json", "{\"res\":500}");
          }
        } else {
          varData.deslayFlag = DEPLOY_ENV_FAILED;
          cfgWifi.priv_serv_addr = "none";
          cfgWifi.priv_serv_port = 0;
          cfgWifi.mqtt_priv_usr = "none";
          cfgWifi.mqtt_priv_pwd = "none";
          request->send(200, "application/json", "{\"res\":500}");
        }
      } else {
        varData.deslayFlag = DEPLOY_ENV_SUCC;
        request->send(200, "application/json", "{\"res\":200}");

        if (update_env_type()) {
          delay(3000);
          // ESP.restart();
        }
      }
    }
  }
}

void parse_web_lang(WebServer *request) {
  if (request->hasArg("web_lang")) {
    cfgWifi.web_lang = request->arg("web_lang");
    Serial.println("LLLLang:" + cfgWifi.web_lang);
    if ((cfgWifi.web_lang == "英文") || (cfgWifi.web_lang == "ENGLISH")) {
      Serial.println("config to english");
      displayVars.web_display_lang = USE_ENGLISH_LANG;
    } else if ((cfgWifi.web_lang == "中文") || (cfgWifi.web_lang == "CHINESE")) {
      Serial.println("config to chinese");
      displayVars.web_display_lang = USE_CHINESE_LANG;
    } else {
      Serial.println("config to english 2");
      displayVars.web_display_lang = USE_ENGLISH_LANG;
    }
    update_web_display_lang();
    Serial.println("update web lang");
    request->send(200, "application/json", "{\"res\":200}");
  } else {
    request->send(200, "application/json", "{\"res\":500}");
  }
}

void parse_reboot_cfg(WebServer *request) {
  if (!request->hasArg("reboot")) {
    request->send(200, "application/json", "{\"res\":500}");
  } else {
    String rebootRet = request->arg("reboot");
    if (rebootRet == "YES") {
      request->send(200, "application/json", "{\"res\":200}");
      delay(2000);
      esp_restart();
    } else {
      request->send(200, "application/json", "{\"res\":500}");
    }
  }
}

void parse_factory_settings(WebServer *request) {
  if (!request->hasArg("factory_settings")) {
    request->send(200, "application/json", "{\"res\":500}");
  } else {
    String factoryRet = request->arg("factory_settings");
    if (factoryRet == "YES") {
      if (factory_settings()) {
        request->send(200, "application/json", "{\"res\":200}");
        varData.deslayFlag = FACTORY_SETTINGS;
      } else {
        request->send(200, "application/json", "{\"res\":500}");
      }
      delay(5000);
      esp_restart();
    } else {
      request->send(200, "application/json", "{\"res\":500}");
    }
  }
}

// 下发处理回调函数
void Config_Callback(AsyncWebServerRequest *request) {
  if (request->hasParam("value"))  // 如果有值下发
  {
    String HTTP_Payload = request->getParam("value")->value();     // 获取下发的数据
    Serial.printf("[%lu]%s\r\n", millis(), HTTP_Payload.c_str());  // 打印调试信息
  }
  request->send(200, "text/plain", "OK");  // 发送接收成功标志符
}
/*
 * 进入AP模式
 */
void initSoftAP() {
  char macTmp[18] = { 0 };
  sprintf(macTmp, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  String AP_SSID = AP_SSID_HEADER + varData.callStaId;
  // WiFi.mode(WIFI_AP);                                         // 配置为AP模式
  if (ap_gw_select_flag == 0) {
    LocalIP.fromString(AP_GW_1);
    GateWay.fromString(AP_GW_1);
    // SubNet.fromString("255.255.255.0");
  } else if (ap_gw_select_flag == 1) {
    LocalIP.fromString(AP_GW_2);
    GateWay.fromString(AP_GW_2);
    // SubNet.fromString("255.255.255.0");
  }
  WiFi.softAPConfig(LocalIP, GateWay, SubNet);  // 设置AP热点IP和子网掩码
  if (WiFi.softAP(AP_SSID.c_str(), AP_PASS))    // 开启AP热点,如需要密码则添加第二个参数
  {
    // 打印相关信息
    Serial.println("ESP-32 SoftAP is right.");
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());                                             // 接入点ip
    Serial.println(String("MAC address = ") + WiFi.softAPmacAddress().c_str());  // 接入点mac
  } else                                                                         // 开启AP热点失败
  {
    Serial.println("WiFiAP Failed");
    delay(1000);
    Serial.println("restart now...");
    ESP.restart();  // 重启复位esp32
  }
}
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String get_mqtt_server_info() {
  String env_m;
  if (varData.envType == ENV_TYPE::PRIVAT) {
    env_m = VALUE_STR_1("PRIVAT");
  } else {
    switch (varData.isAxEnv) {
      case ALI_ENV:
        {
          env_m = VALUE_STR_1("ALIYUN");
          break;
        }
      case AX_BUILD_ENV:
        {
          env_m = VALUE_STR_1("AX_CN");
          break;
        }
      case AX_BUILD_ENV_GLBL:
        {
          env_m = VALUE_STR_1("AX_GLOBAL");
          break;
        }
      case AX_BUILD_ENV_EU:
        {
          env_m = VALUE_STR_1("AX_EUROPE");
          break;
        }
      default:
        {
          env_m = VALUE_STR_1("UNKNOWN");
          break;
        }
    }
  }
  return env_m;
}

String get_deploy_env_info() {
  String env_m;
  switch (varData.envType) {
    case UNCFG:
      {
        if (displayVars.web_display_lang == USE_CHINESE_LANG)
          env_m = VALUE_STR_1("未配置");
        else
          env_m = VALUE_STR_1("unconfig");
        break;
      }
    case PRIVAT:
      {
        if (displayVars.web_display_lang == USE_CHINESE_LANG)
          env_m = VALUE_STR_1("私有化环境");
        else
          env_m = VALUE_STR_1("PRIVAT");
        break;
      }
    case DEVELOPMENT:
      {
        if (displayVars.web_display_lang == USE_CHINESE_LANG)
          env_m = VALUE_STR_1("开发环境");
        else
          env_m = VALUE_STR_1("DEVELOPMENT");
        break;
      }
    case PRODUCT:
      {
        if (displayVars.web_display_lang == USE_CHINESE_LANG)
          env_m = VALUE_STR_1("生产环境");
        else
          env_m = VALUE_STR_1("PRODUCT");
        break;
      }
    case FOREIGN:
      {
        if (displayVars.web_display_lang == USE_CHINESE_LANG)
          env_m = VALUE_STR_1("海外环境");
        else
          env_m = VALUE_STR_1("FOREIGN");
        break;
      }
    case JAPANESE:
      {
        if (displayVars.web_display_lang == USE_CHINESE_LANG)
          env_m = VALUE_STR_1("日本环境");
        else
          env_m = VALUE_STR_1("JAPANESE");
        break;
      }
    default:
      break;
  }
  return env_m;
}

void init_web_infos() {
  if (displayVars.web_display_lang == USE_CHINESE_LANG) {
    webInfos.sn = KEY_1("sn") + VALUE_DEF(varData.callStaId);
    webInfos.version = KEY_1("version") + VALUE_DEF(VERSION);
    webInfos.web_lang = KEY_1("web_lang") VALUE_STR_1("中文");
    webInfos.deploy_lang = KEY_1("deploy_lang");
    if (displayVars.user_lang == USE_CHINESE_LANG) {
      webInfos.deploy_lang += VALUE_STR_1("中文");
    } else if (displayVars.user_lang == USE_JAPANESE_LANG) {
      webInfos.deploy_lang += VALUE_STR_1("JAPANESE");
    } else {
      webInfos.deploy_lang += VALUE_STR_1("ENGLISH");
    }
    webInfos.ssid = KEY_1("SSID") + VALUE_DEF(varData.wifiName);
    webInfos.password = KEY_1("Password") + VALUE_DEF(varData.wifiPwd);
    webInfos.ip = KEY_1("ip") + VALUE_DEF(WiFi.localIP().toString());
    webInfos.rssi = KEY_1("rssi") + VALUE_DEF(WiFi.RSSI());
    webInfos.deploy_env = KEY_1("deploy_type");
    webInfos.deploy_env += get_deploy_env_info();
    if (varData.envType == PRIVAT) {
      webInfos.priv_proto = KEY_1("dagreement_type") + VALUE_DEF(varData.priv_proto);
      webInfos.priv_addr = KEY_1("priv_addr") + VALUE_DEF(varData.priv_addr);
      webInfos.priv_port = KEY_1("priv_port") + VALUE_DEF(varData.priv_port);
      webInfos.priv_usr = KEY_1("priv_usr") + VALUE_DEF(varData.priv_usr);
      webInfos.priv_pwd = KEY_1("priv_pwd") + VALUE_DEF(varData.priv_pwd);
    }

    webInfos.mqtt_server = KEY_1("mqtt_type");
    webInfos.mqtt_server += get_mqtt_server_info();
    webInfos.mqtt_server_ip = KEY_1("mqtt_srv_ip");
    webInfos.mqtt_online = KEY_1("mqtt_online");
    webInfos.dns = KEY_1("dns_server");

    webInfos.ap_gw = KEY_1("ap_gw") + VALUE_DEF(GateWay.toString());
    if (strncmp(varData.wifiProto, "WPA2_ENTERPRISE", 15) == 0) {
      webInfos.wpa2EnterpriseAuthMethod = KEY_1("wpa2EnterpriseAuthMethod") + VALUE_DEF(cfgWifi.method);
      webInfos.wpa2EnterpriseAuthIdentity = KEY_1("wpa2EnterpriseAuthIdentity") + VALUE_DEF(cfgWifi.wpa2_identity);
      webInfos.wpa2EnterpriseAuthUsername = KEY_1("wpa2EnterpriseAuthUsername") + VALUE_DEF(cfgWifi.wpa2_username);
    }
  } else {
    webInfos.sn = KEY_1("sn") + VALUE_DEF(varData.callStaId);
    webInfos.version = KEY_1("version") + VALUE_DEF(VERSION);
    webInfos.web_lang = KEY_1("web_lang") VALUE_STR_1("ENGLISH");
    webInfos.deploy_lang = KEY_1("deploy_lang");
    if (displayVars.user_lang == USE_CHINESE_LANG) {
      webInfos.deploy_lang += VALUE_STR_1("中文");
    } else if (displayVars.user_lang == USE_JAPANESE_LANG) {
      webInfos.deploy_lang += VALUE_STR_1("JAPANESE");
    } else {
      webInfos.deploy_lang += VALUE_STR_1("ENGLISH");
    }

    webInfos.ssid = KEY_1("SSID") + VALUE_DEF(varData.wifiName);
    webInfos.password = KEY_1("Password") + VALUE_DEF(varData.wifiPwd);
    webInfos.ip = KEY_1("ip") + VALUE_DEF(WiFi.localIP().toString());
    webInfos.rssi = KEY_1("rssi") + VALUE_DEF(WiFi.RSSI());
    webInfos.deploy_env = KEY_1("deploy_type");
    webInfos.deploy_env += get_deploy_env_info();

    if (varData.envType == PRIVAT) {
      webInfos.priv_proto = KEY_1("dagreement_type") + VALUE_DEF(varData.priv_proto);
      webInfos.priv_addr = KEY_1("priv_addr") + VALUE_DEF(varData.priv_addr);
      webInfos.priv_port = KEY_1("priv_port") + VALUE_DEF(varData.priv_port);
      webInfos.priv_usr = KEY_1("priv_usr") + VALUE_DEF(varData.priv_usr);
      webInfos.priv_pwd = KEY_1("priv_pwd") + VALUE_DEF(varData.priv_pwd);
    }

    webInfos.mqtt_server = KEY_1("mqtt_type");
    webInfos.mqtt_server += get_mqtt_server_info();
    webInfos.mqtt_server_ip = KEY_1("mqtt_srv_ip");
    webInfos.mqtt_online = KEY_1("mqtt_online");
    webInfos.dns = KEY_1("dns_server");
    webInfos.ap_gw = KEY_1("ap_gw") + VALUE_DEF(GateWay.toString());
    if (strncmp(varData.wifiProto, "WPA2_ENTERPRISE", 15) == 0) {
      webInfos.wpa2EnterpriseAuthMethod = KEY_1("wpa2EnterpriseAuthMethod") + VALUE_DEF(cfgWifi.method);
      webInfos.wpa2EnterpriseAuthIdentity = KEY_1("wpa2EnterpriseAuthIdentity") + VALUE_DEF(cfgWifi.wpa2_identity);
      webInfos.wpa2EnterpriseAuthUsername = KEY_1("wpa2EnterpriseAuthUsername") + VALUE_DEF(cfgWifi.wpa2_username);
    }
  }
}

/* Style */
String style =
  "<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
  "input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
  "#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
  "#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
  "form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
  ".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

/* Login page */
String loginIndex =
  "<form name=loginForm>"
  "<h1>Pager OTA Login</h1>"
  "<input name=userid placeholder='User ID'> "
  "<input name=pwd placeholder=Password type=Password> "
  "<input type=submit onclick=check(this.form) class=btn value=Login></form>"
  "<script>"
  "function check(form) {"
  "if(form.userid.value=='admin' && form.pwd.value=='admin')"
  "{window.open('/serverIndex')}"
  "else"
  "{alert('Error Password or Username')}"
  "}"
  "</script>"
  + style;

/* Server Index Page */
String serverIndex =
  "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
  "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
  "<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
  "<label id='file-input' for='file'>   Choose file...</label>"
  "<input type='submit' class=btn value='Update'>"
  "<br><br>"
  "<div id='prg'></div>"
  "<br><div id='prgbar'><div id='bar'></div></div><br></form>"
  "<script>"
  "function sub(obj){"
  "var fileName = obj.value.split('\\\\');"
  "document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
  "};"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  "$.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "$('#bar').css('width',Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!') "
  "},"
  "error: function (a, b, c) {"
  "}"
  "});"
  "});"
  "</script>"
  + style;

void initWebServer() {
  init_web_infos();
  initSoftAP();

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "content-type");
  // server_update.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  //                  { request->send(200, "text/plain", "Hi! I am AX_PAGER_MODULE.\rS/N:" + varData.callStaId + "\rversion:" + VERSION); });
  // AsyncElegantOTA.begin(&server_update, "autoxing", "123456"); // Start ElegantOTA
  // server_update.begin();

  /*use mdns for host name resolution*/
  // if (!MDNS.begin(host))
  // { // http://esp32.local
  //   Serial.println("Error setting up MDNS responder!");
  //   while (1)
  //   {
  //     delay(1000);
  //   }
  // }
  // Serial.println("mDNS responder started");

  server_cfg.on("/", HTTP_GET, []() {
    server_cfg.send(200, "text/html", router_index_html);
  });
  server_cfg.on("/get_infos", HTTP_GET, []() {
    send_infos(&server_cfg);
  });

  // /apgw_select?ap_gw=0
  server_cfg.on("/apgw_select", HTTP_GET, []() {
    parse_ap_gw(&server_cfg);
  });
  server_cfg.on("/wificonfad", HTTP_GET, []() {
    parse_wifi_cfg(&server_cfg);
  });
  server_cfg.on("/private_deploy", HTTP_GET, []() {
    parse_private_deploy(&server_cfg);
  });

  server_cfg.on("/web_lang_select", HTTP_GET, []() {
    parse_web_lang(&server_cfg);
  });

  server_cfg.on(
    "/update", HTTP_POST, []() {
      server_cfg.sendHeader("Connection", "close");
      server_cfg.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    },
    []() {
      HTTPUpload &upload = server_cfg.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {  //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        /* flashing firmware to ESP*/
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {  //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
      }
    });
  server_cfg.on("/reboot", HTTP_GET, []() {
    parse_reboot_cfg(&server_cfg);
  });
  server_cfg.on("/factory_settings", HTTP_GET, []() {
    parse_factory_settings(&server_cfg);
  });
  // server_cfg.onNotFound(notFound);
  server_cfg.onNotFound([]() {
    server_cfg.send(404, "text/plain", "404: Not found");
  });
  server_cfg.begin();  // 初始化HTTP服务器
}

void pthread_ap(void *) {
  initWebServer();
  while (1) {
    server_cfg.handleClient();
    delay(10);
  }
}
