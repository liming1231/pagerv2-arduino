#include "ax_common.h"
#include "soc/soc.h"
#include <esp_wifi.h>
#include <esp_task_wdt.h>

void setup(void)
{
  // 立即禁用看门狗
  esp_task_wdt_deinit();

  Serial.begin(115200);

  delay(1000);

  Serial.println("=== ESP32-C5 Start Config ===");

  // 逐步初始化，每步都打印调试信息
  Serial.printf("=== 1.Read MAC Addr:");
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  sprintf(mac_s, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.printf("%s ===\r\n", mac_s);

  Serial.println("=== 2. Init GPIO ===");
  init_gpio_pins();

  Serial.println("=== 3.Make A Buzzer ===");
  make_a_buzzer();

  Serial.println("=== 4. Init Vars ===");
  init_var_data();

  // Serial.println("=== 5. Set WiFi Power ===");
  // ESP32-C5 使用新的WiFi功率设置方法
  // esp_wifi_set_max_tx_power(80); // 旧方法，ESP32-C5不支持

  Serial.println("=== 6. Read Flash Config ===");
  read_flash_config();

  Serial.println("=== 7. Read Run Env ===");
  get_run_env();

  Serial.println("=== 8. Config WiFi DNS ===");
  cfg_wifi_dns();

  Serial.println("=== 9. Init Env ===");
  init_env();

  Serial.print("=== 10. Version: ");
  Serial.printf("%s ===\r\n", VERSION.c_str());

  Serial.println("=== 11. Init Language ===");
  init_display_language();

  Serial.println("=== 12. Init SSD1306 ===");
  init_ssd1306();

  Serial.println("=== 13. Check Facktory ===");
  check_if_need_factory();

  Serial.println("=== 14. Set WiFi Mode ===");
  // ESP32-C5: 简单的WiFi模式设置，忽略协议检测警告
  WiFi.mode(WIFI_AP_STA); // 设置为AP+STA模式 ，有告警（wifi:can not get wifi protocol under WiFi band mode WIFI_BAND_MODE_AUTO, please use esp_wifi_get_protocols instead）
                          // 据说告警不影响使用
  delay(100);             // 给WiFi初始化更多时间
  // 初始化WiFi后再设置功率
  Serial.println("=== 5. Set WiFi Power ===");
  WiFi.setTxPower(WIFI_POWER_19_5dBm); // 使用新的功率设置方法

  Serial.println("=== 15. Create AP Thread ===");
  // 先注释掉这个
  xTaskCreate(pthread_ap, "pthread_ap", 4096, NULL, 7, NULL);

  Serial.println("=== end Config ===");
}

void loop(void)
{
  static unsigned long lastTimestep = 0;
  unsigned long nowTimestep = millis();

  if (nowTimestep - lastTimestep >= 5000)
  {
    lastTimestep = nowTimestep;
    Serial.println("Trigger LED...");
    led_trigger();
  }
  delay(100);
}