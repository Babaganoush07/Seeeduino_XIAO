// created by upir, 2022
// youtube channel: https://www.youtube.com/upir_upir
// FULL TUTORIAL: https://youtu.be/NPfaLKKsf_Q 
// Wokwi starting project: https://wokwi.com/arduino/projects/300867986768527882

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define LED_PIN 2
#define photoTransistor A8

int photoTransistorValue = 0;
int brightness = 0;

char buffer[20];       // helper buffer for converting values into C-style string (array of chars)
int string_width;      // helper value for string widths

float pixel_x = 0;     // x pos for pixel
float pixel_y = 0;     // y pos for pixel
float line_x = 0;      // x pos for line end
float line_y = 0;      // y pos for line end
float text_x = 0;      // x pos for text
float text_y = 0;      // y pos for text

int center_x = 64;     // x center of the knob 
int center_y = 108;    // y center of the knob (outside of the screen)
int radius_pixel = 92; // radius for pixel tickmarks
int radius_line = 87;  // radius for line end
int radius_text = 75;  // radius for text

int angle;             // angle for the individual tickmarks
int tick_value;        // numeric value for the individual tickmarks

byte precalculated_x_radius_pixel[180]; // lookup table to prevent expensive sin/cos calculations
byte precalculated_y_radius_pixel[180]; // lookup table to prevent expensive sin/cos calculations

void setup(void) {
  u8g2.begin();
  u8g2.setFont(u8g2_font_HelvetiPixel_tr);  // Select 8 pixel high font
  u8g2.setDrawColor(1);                     // Set color to white
  for (int i = 0; i < 180; i++) {    // pre-calculate x and y positions into the look-up tables
     precalculated_x_radius_pixel[i] =  sin(radians(i-90)) * radius_pixel + center_x; 
     precalculated_y_radius_pixel[i] = -cos(radians(i-90)) * radius_pixel + center_y;      
  }
  pinMode(LED_PIN, OUTPUT);
}

void loop(void) {
  u8g2.clearBuffer();    // Clear the internal memory
  
  u8g2.setDrawColor(1);  // Set color to white

  for(int i=-48; i<=48; i=i+3){
    angle = i + (photoTransistorValue*3) % 3;                // final angle for the tickmark
    tick_value = round(photoTransistorValue + angle/3.0);  // get number value for each tickmark
    
    pixel_x = precalculated_x_radius_pixel[angle+90];              // get x value from lookup table
    pixel_y = precalculated_y_radius_pixel[angle+90];              // get y value from lookup table

    if (pixel_x > 0 && pixel_x < 128 && pixel_y > 0 && pixel_y < 64) {  // only draw inside of the screen
      if(tick_value >= 0 && tick_value <= 100) {  // only draw tickmarks between values 0-100%, could be removed when using rotary controller
        if (tick_value % 10 == 0) {  // Draw LINES
          line_x = sin(radians(angle)) * radius_line + center_x;
          line_y = -cos(radians(angle)) * radius_line + center_y;
          u8g2.drawLine(pixel_x, pixel_y, line_x, line_y);

          text_x = sin(radians(angle)) * radius_text + center_x;
          text_y = -cos(radians(angle)) * radius_text + center_y;
          itoa(tick_value, buffer, 10);                            // convert integer to string
          string_width = u8g2.getStrWidth(buffer);                  // get string width
          u8g2.drawStr(text_x - string_width/2, text_y, buffer);    // draw text - tickmark value            
        }else{  // Draw PIXELS tic marks
          u8g2.drawPixel(pixel_x, pixel_y);
        }
      }
    }
  }
  
  itoa(photoTransistorValue,buffer,10);               // Convert it to a character string
  sprintf(buffer, "%s%s", buffer, "%");   // Combine two strings
  string_width = u8g2.getStrWidth(buffer);              // Get the pixel width of string
  
  u8g2.drawRBox(64-(string_width+4)/2, 0, string_width+4, 11, 2);  // Draw background rounded rectangle
  u8g2.drawTriangle( 64-3, 11,   64+4, 11,   64, 15);      // Draw small arrow below the rectangle
  u8g2.setDrawColor(0);                               // Set color to black
  u8g2.drawStr(64-string_width/2, 10, buffer);  // Display value as a string
  u8g2.sendBuffer();                               // Transfer internal memory to the display

  photoTransistorValue = map(analogRead(photoTransistor), 0, 4095, 0, 100);  // Read the value
  brightness = map(photoTransistorValue, 0, 4095, 0, 255);
  analogWrite(LED_PIN, brightness);        // Power the LED
}
