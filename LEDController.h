#pragma once

#include "Color.h"

class LEDController
{
public:
	static LEDController* Instance;
	LEDController();

	void UpdateLeds(Color* colorBuffer, int numLeds);
	void ShiftOut8Bits(int clockPin, int dataPin, char c);

private:
	int m_clockPin1;
	int m_clockPin2;

	int m_dataPin2;
	int m_dataPin1;

	bool m_isSetup;
};