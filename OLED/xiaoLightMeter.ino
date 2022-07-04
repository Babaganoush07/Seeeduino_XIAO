#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define LED_PIN 2
#define PhotoTransistor A8

int phtrAnalogValue = 0;
int brightness = 0;
int barValue = 0;
int percent = 0;


void setup(void) {
  u8g2.begin();
  u8g2.enableUTF8Print();  // enable UTF8 support for the Arduino print() function
  analogReadResolution(12); // Set to 12, xaio defaults to 10
  pinMode(LED_PIN, OUTPUT);
}

void loop(void) {

  phtrAnalogValue = analogRead(PhotoTransistor);       // Get Values
  brightness = map(phtrAnalogValue, 0, 4095, 0, 255);  // Map Values
  barValue = map(phtrAnalogValue, 0, 4095, 0, 124);
  percent = map(phtrAnalogValue, 0, 4095, 0, 100);
  if(barValue < 8){barValue = 8;}                      // Adjust progress bar value
  
  analogWrite(LED_PIN, brightness);        // Power the LED

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_HelvetiPixel_tr);
  u8g2.setCursor(25, 10);
  u8g2.print("LIGHT METER");
  
  u8g2.setCursor(0, 20);
  u8g2.print("Photo Transistor: ");
  u8g2.print(phtrAnalogValue);
  
  u8g2.setCursor(0, 30);
  u8g2.print("LED Brightness:   ");
  u8g2.print(brightness);

  u8g2.setFontMode(1);  /* activate transparent font mode */
  u8g2.setDrawColor(1); /* color 1 for the box */
  u8g2.drawRFrame(0, 35, 128, 12, 6);
  u8g2.drawRBox(2, 37, barValue, 8, 4);

  u8g2.setDrawColor(2);
  u8g2.setCursor(55, 45);
  u8g2.print(percent);
  u8g2.print("%");
  u8g2.setCursor(0, 61);
  u8g2.print("0%                 100%");
  u8g2.sendBuffer();
}
