# OLED 128x64


## General Information
- I picked up this HiLetgo OLED from [Amazon](https://www.amazon.com/dp/B01MRR4LVE?psc=1&ref=ppx_yo2ov_dt_b_product_details)
- It's listed as: HiLetgo 1.3" IIC I2C Serial 128x64 SSH1106 SSD1306 OLED LCD Display LCD Module for Arduino AVR PIC STM32
- But it does **_NOT use SSD1306_**.


## Setup
- With the display having a SSH1106 chip I used the [u8g2 library](https://github.com/olikraus/U8g2_Arduino).
- It can be found in the Libraries Manager.
- I got it working with this constructor: `U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);`

## HELLO WORLD!
```
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup(void) {
  u8g2.begin();
  u8g2.enableUTF8Print();  // enable UTF8 support for the Arduino print() function
}

void loop(void) {
  u8g2.clearBuffer();                       // clear the internal memory
  u8g2.setFont(u8g2_font_HelvetiPixel_tr);  // choose a suitable font
  u8g2.drawStr(0,10,"Hello World!");        // write something to the internal memory
  u8g2.sendBuffer();                        // transfer internal memory to the display
  delay(1000);  
}
```

