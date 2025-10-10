#ifndef __AX_SSD1306_H__
#define __AX_SSD1306_H__

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

enum
{
    LINE_1 = 1,
    LINE_2 = 2,
    LINE_3 = 3,
    LINE_4 = 4
};

extern uint8_t mac[6];
extern char mac_s[16];

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

typedef struct _display_field
{
    String firstScreen;
    String unConnSts;
    String connectedSts;
    String idleSts;
    String cfgWifiSts;
    String cfgWifiSucc;
    String callingSts;
    String callSuccSts;
    String callRejectedSts;
    String cancelCallSts;
    String callCanceledSts;
    String callCancelRejected;
    String positionUnbound;
    String positionBinding;
    String positionBindFail;
    String positionBindSucc;
    String positionBindIncomplete;
    String taskWaitingSts;
    String remainedHeader;
    String remainedTail;
    String remainedTails;
    String robotTag;
    String taskTag;
    String rbtOnTheWay;
    String distanceHeader;
    String distanceTail;
    String rbtArrived;
    String taskFailed;
    String accident;
    String taskCanceled;
    String cfgDeploySucc;
    String cfgDeployFail;
    String cfg_factory_settings;
} display_field;

extern display_field displayField;

void init_ssd1306(void);

void display_test(void);

void init_error_code(void);

void init_display_language(void);

#endif
