#pragma once

#include "Color.h"

class LEDController
{
public:
	static LEDController* Instance;
	LEDController(int clockPin, int dataPin);

	void SetPins(int clockPin, int dataPin);
	void UpdateLeds(Color* colorBuffer, int numLeds);
	void ShiftOut8Bits(char c);

private:
	int m_clockPin;
	int m_dataPin;
	bool m_isSetup;
};