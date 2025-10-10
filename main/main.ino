// 这会让编译器使用项目目录下的partitions.csv
// #pragma message "Using custom partition table"
// #define PARTITION_TABLE_OVERRIDE
#include "ax_common.h"
#include "soc/soc.h"
// #include "soc/rtc_cntl_reg.h"
#include <esp_wifi.h>

void setup(void) {
  delay(500);
  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  sprintf(mac_s, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.begin(115200);
  init_gpio_pins();
  make_a_buzzer();
  init_var_data();
  esp_wifi_set_max_tx_power(80);
  read_flash_config();
  get_run_env();
  cfg_wifi_dns();
  init_env();
  Serial.println(VERSION);

  init_display_language();
  init_ssd1306();

  check_if_need_factory();

  WiFi.mode(WIFI_AP_STA);
}

void loop(void) {
  // display_test();
  delay(200);
}


