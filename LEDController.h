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

	void UpdateLeds(Color* colorBuffer, float deltaTime);
	void ShiftOut8Bits(int clockPin, int dataPin, uint8_t c);
	void SetFadeTime(int fadeTime)
	{
		m_fadeTimeMS = fadeTime;
	}

private:
	bool m_isSetup;
	int m_fadeTimeMS;
	Color *m_prevColorBuffer;
};