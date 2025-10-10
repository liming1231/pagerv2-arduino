#include "ax_common.h"

void init_gpio_pins(void) {
  pinMode(BROADLED, OUTPUT);
  pinMode(CALL_STATUS_LED, OUTPUT);
  pinMode(BUZZER_SIG, OUTPUT);

  // pinMode(CALL_BUTTON, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(CALL_BUTTON), interruptHandler, FALLING);
  pinMode(CALL_BUTTON, INPUT_PULLDOWN);
  pinMode(DECALL_BUTTON, INPUT_PULLDOWN);

  digitalWrite(BROADLED, LOW);
  digitalWrite(CALL_STATUS_LED, LOW);
  digitalWrite(BUZZER_SIG, LOW);
}

void make_a_buzzer(void) {
  digitalWrite(BUZZER_SIG, HIGH);
  delay(500);
  digitalWrite(BUZZER_SIG, LOW);
}
