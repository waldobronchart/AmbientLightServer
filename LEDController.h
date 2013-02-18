#pragma once

#include "Color.h"
#include "Logging.h"

class LEDController
{
public:
	static LEDController* Instance;
	LEDController();

	~LEDController()
	{
		LOG_INFO("delete prev buffer");
		delete[] m_prevColorBuffer;
	}

	void UpdateLeds(Color* colorBuffer);
	void ShiftOut8Bits(int clockPin, int dataPin, char c);

private:
	bool m_isSetup;
	Color *m_prevColorBuffer;
};