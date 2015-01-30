#ifndef LiquidOLED_h
#define LiquidOLED_h

#include "Print.h"

// base commands
#define OLED_CLEARDISPLAY 0b00000001
#define OLED_RETURNHOME   0b00000010
#define OLED_ENTRY_MODE   0b00000100
#define OLED_FUNCTION_SET 0b00111000 // 8-bit only
#define OLED_DISPLAY      0b00001000
#define OLED_SETDDRAMADDR 0b10000000

// Entry Mode flags...
#define OLED_CURSOR_INCREMENT  0b00000010
#define OLED_CURSOR_DECCREMENT 0b00000000
#define OLED_SHIFT_RIGHT       0b00000001
#define OLED_SHIFT_LEFT        0b00000000

// Function Set flags...
#define OLED_ENGLISH_JAPANESE 0b00000000
#define OLED_WESTERN_EURO_1   0b00000001
#define OLED_ENGLISH_RUSSIAN  0b00000010
#define OLED_WESTERN_EURO_2   0b00000011

// Display Control flags...
#define OLED_DISPLAY_ON  0b00000100
#define OLED_DISPLAY_OFF 0b00000000
#define OLED_CURSOR_ON   0b00000010
#define OLED_CURSOR_OFF  0b00000000
#define OLED_BLINK_ON    0b00000001
#define OLED_BLINK_OFF   0b00000000

class LiquidOLED : public Print {
public:
	LiquidOLED();
	LiquidOLED(uint8_t _clock, uint8_t _mosi, uint8_t _select);

	void setup();

	// implementation for Print super
	virtual size_t write(uint8_t character);

    void clear();
    void home();

    void blinkOn();
    void blinkOff();

    void cursorOn();
    void cursorOff();

    void displayOn();
    void displayOff();

	void setCursor(uint8_t col, uint8_t row);

	void setEntryMode(uint8_t entryMode);
	void setFunctionSet(uint8_t functionSet);

private:
	void command(uint8_t value);
	void send(uint8_t value, uint8_t mode);
	void toggleClk(void);

	uint8_t _displaycontrol;

	uint8_t _clock;
	uint8_t _mosi;
	uint8_t _select;
};

#endif
