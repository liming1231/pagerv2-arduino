#ifndef __AX_GPIO_H__
#define __AX_GPIO_H__

#include <Arduino.h>

// 呼叫器需要3个输入两个输出IO
// 呼叫按钮和取消按钮
// 主板运行灯，呼叫指示灯，蜂鸣器
// IO6/7/8/24/25

#define BROADLED 6
#define CALL_STATUS_LED 7
#define BUZZER_SIG 8

#define CALL_BUTTON 24
#define DECALL_BUTTON 25




void init_gpio_pins(void);
void make_a_buzzer(void);
void led_trigger(void);

#endif