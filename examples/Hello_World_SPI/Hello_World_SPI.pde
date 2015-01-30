#include "LiquidOLED.h"

/*
 *  Only three data pins are needed (SCK, MOSI, and SS)
 *
 *  Winstar/NewHaven 16x2 OLED pins:
 *    1  - GND
 *    2  - VIN
 *    12 - SCL (SCK)
 *    14 - SDI (MOSI)
 *    16 - CS (SS)
 * 
 *  Map Arduino pins:
 *  
 *     LiquidOLED oled = LiquidOLED(); // use hardware default SPI pins
 *     or
 *     LiquidOLED oled = LiquidOLED(clock, data, select); // any digital
 */

const int clock  = 7; // SCK
const int data   = 8; // SDI/MOSI
const int select = 9; // SS/CS

LiquidOLED oled = LiquidOLED(clock, data, select);

void setup() {
  oled.setup();

  oled.print("Hello World!");

  delay(1500);

  oled.clear();
  oled.home();
  oled.print("Hello Worlds: ");
}

void loop() {
  if ((millis()/1000) % 5 == 0) {
    oled.setCursor(14, 0);
    oled.print(millis()/1000);
  }

  oled.setCursor(0, 1);
  oled.print(millis()/1000);

  delay(500);
}