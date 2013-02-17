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
	bool m_isSetup;
};