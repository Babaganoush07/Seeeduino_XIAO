#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

// Wokwi Constructor
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 21, /* data=*/ 22);   // ESP32 Thing, HW I2C with pin remapping
// Arduino Constructor
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define photoTransistor A8

int photoTransistorValue = 0;

int string_width;
char buffer[20];
int line_x;

void setup(void) {
  u8g2.begin();
  Serial.begin(9600);
  u8g2.setFont(u8g2_font_HelvetiPixel_tr);
  u8g2.setDrawColor(1);
}

void loop(void) {
  u8g2.clearBuffer();
  u8g2.setDrawColor(1);
  
  for(int i=0; i<=200; i+=5){
    line_x = (photoTransistorValue/5+64) - i;
    if (line_x > 0 && line_x < 128) {
      if(i %20 == 0){
        u8g2.drawLine(line_x, 18, line_x, 28);
        itoa(i/2,buffer,10);
        string_width = u8g2.getStrWidth(buffer);
        u8g2.drawStr(line_x - string_width/2, 40, buffer);
      }else{
        u8g2.drawLine(line_x, 18, line_x, 23);
      }
    }
  }

  itoa(photoTransistorValue/10,buffer,10);
  sprintf(buffer, "%s%s", buffer, "%");
  string_width = u8g2.getStrWidth(buffer);
  u8g2.drawRBox(64-(string_width+4)/2, 0, string_width+4, 11, 2);
  u8g2.drawTriangle( 64-3, 11,   64+4, 11,   64, 15);
  u8g2.setDrawColor(0);
  u8g2.drawStr(64-string_width/2, 10, buffer);
  u8g2.sendBuffer();

  //analogReadResolution(10); for 1023
  photoTransistorValue = map(analogRead(photoTransistor), 0, 4095, 0, 1000);  // Read the value
}
