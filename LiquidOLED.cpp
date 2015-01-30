#include "LiquidOLED.h"
#include "Arduino.h"

/**
 * LiquidOLED.cpp
 *
 *  Created on: Jan 25, 2015
 *      Author: Troy Hamsher
 *
 *  LiquidOLED is a free and open-source Arduino Character OLED library for SPI.
 *
 *  Only three data pins are needed (CLK, MOSI, and SS) which correspond to OLED pins:
 *
 *  Winstar/NewHaven 16x2 OLED pins:
 *    1  - GND
 *    2  - VIN
 *    12 - SCL (SCK)
 *    14 - SDI (MOSI)
 *    16 - CS (SS)
 *
 *  Since this is software-SPI, these pins can be mapped to any digital pin on your
 *  Arduino. Or, just use your hardware default matching pins and default constructor.
 *
 *  This API has been tested with the Winstar 16x2 Blue OLED sold by Adafruit.  It's
 *  based on the specification sheet provided by NewHaven for their OLED SPI interface.
 *
 *  These OLED devices require jumpers to be moved/soldered to enable the SPI interface!
 *  This blog has a good description of the jumper changes (really only two jumpers
 *  to move):
 *
 *  http://blog.adafruit.com/2013/03/05/using-an-adafruit-oled-16x2-character-display-in-spi-mode/
 *
 *  The following references were used:
 *
 *     http://blog.adafruit.com/2013/03/05/using-an-adafruit-oled-16x2-character-display-in-spi-mode/
 *     http://www.picaxeforum.co.uk/entry.php?194-Interface-Winstar
 *     http://www.newhavendisplay.com/specs/NHD-0216KZW-AB5.pdf
 *     Adafruit LiquidCrystal library,
 */

const int max_rows = 4;
const int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

/**
 * Use the hardware default SPI pins.
 */
LiquidOLED::LiquidOLED() {
	_displaycontrol = OLED_DISPLAY_ON | OLED_CURSOR_OFF | OLED_BLINK_OFF;

	// use default pins based on the processor
	_clock = SCK;
	_mosi = MOSI;
	_select = SS;
}

/**
 * Specify your own pin mapping.
 */
LiquidOLED::LiquidOLED(uint8_t clock, uint8_t mosi, uint8_t select) {
	_displaycontrol = OLED_DISPLAY_ON | OLED_CURSOR_OFF | OLED_BLINK_OFF;

	// map our own pins...
	_clock = clock;
	_mosi = mosi;
	_select = select;
}

/**
 * Setup the OLED.
 */
void LiquidOLED::setup() {
	pinMode(_select, OUTPUT);
	pinMode(_clock, OUTPUT);
	pinMode(_mosi, OUTPUT);

	digitalWrite(_select, HIGH);
	digitalWrite(_clock, HIGH);
	digitalWrite(_mosi, LOW);

	delay(500); // wait for power stabilization

	setFunctionSet(OLED_ENGLISH_JAPANESE);
	displayOn();
	clear();
	home();
	setEntryMode(OLED_CURSOR_INCREMENT | OLED_SHIFT_LEFT);
}

/**
 * Moves the cursor to the column and row (zero based)
 */
void LiquidOLED::setCursor(uint8_t col, uint8_t row) {
	if (row > max_rows) {
		row = max_rows - 1;
	}
	command(OLED_SETDDRAMADDR | (col + row_offsets[row]));
}

void LiquidOLED::clear()
{
  command(OLED_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidOLED::home()
{
  command(OLED_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidOLED::displayOn() {
	_displaycontrol |= OLED_DISPLAY_ON;
	command(OLED_DISPLAY | _displaycontrol);
	delayMicroseconds(500);
}

void LiquidOLED::displayOff() {
	_displaycontrol &= ~OLED_DISPLAY_ON;
	command(OLED_DISPLAY | _displaycontrol);
	delayMicroseconds(500);
}

void LiquidOLED::cursorOn() {
	_displaycontrol |= OLED_CURSOR_ON;
	command(OLED_DISPLAY | _displaycontrol);
	delayMicroseconds(500);
}

void LiquidOLED::cursorOff() {
	_displaycontrol &= ~OLED_CURSOR_ON;
	command(OLED_DISPLAY | _displaycontrol);
	delayMicroseconds(500);
}

void LiquidOLED::blinkOn() {
	_displaycontrol |= OLED_BLINK_ON;
	command(OLED_DISPLAY | _displaycontrol);
	delayMicroseconds(500);
}

void LiquidOLED::blinkOff() {
	_displaycontrol &= ~OLED_BLINK_ON;
	command(OLED_DISPLAY | _displaycontrol);
	delayMicroseconds(500);
}

void LiquidOLED::setFunctionSet(uint8_t functionSet) {
	command(OLED_FUNCTION_SET | functionSet);
	delayMicroseconds(500);
}

void LiquidOLED::setEntryMode(uint8_t entryMode) {
	command(OLED_ENTRY_MODE | entryMode);
	delayMicroseconds(500);
}

/**
 * Overrides Print.write() in order to inherit nifty print functions.
 */
size_t LiquidOLED::write(uint8_t character) {
	send(character, HIGH);
	return 1;
}

// private member functions...

/**
 * Sends a command value to the OLED.
 */
void LiquidOLED::command(uint8_t value) {
	send(value, LOW);
}

/**
 * Toggles the clock pin with appropriate timing.
 */
void LiquidOLED::toggleClk(void) {
	digitalWrite(_clock, LOW);
	delayMicroseconds(1);
	digitalWrite(_clock, HIGH);
	delayMicroseconds(1);    // enable pulse must be >450ns
	digitalWrite(_clock, LOW);
	delayMicroseconds(100);  // commands need > 37us to settle
}

/**
 * Write either a command (mode=LOW) or data (mode=HIGH)
 */
void LiquidOLED::send(uint8_t value, uint8_t mode) {
	digitalWrite(_select, LOW); // grab our device

	// Write two bits (0 0 for commands, 1 0 for data)
	digitalWrite(_mosi, mode);
	toggleClk();
	digitalWrite(_mosi, LOW);
	toggleClk();

	// Use the built-in shift function to write the value
	shiftOut(_mosi, _clock, MSBFIRST, value);

	digitalWrite(_select, HIGH); // release the device
}
