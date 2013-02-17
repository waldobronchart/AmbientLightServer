#pragma once

#include "Color.h"

class LEDController
{
public:
	static LEDController* Instance;
	LEDController(int clockPin, int dataPin);

	void SetPins(int clockPin, int dataPin);
	void UpdateLeds(Color* colorBuffer, int numLeds);
	void ShiftOut8Bits(int clockPin, int dataPin, char c);

private:
	int m_clockPin;
	int m_dataPin;

	int m_clockPin2;
	int m_dataPin2;

	bool m_isSetup;
};