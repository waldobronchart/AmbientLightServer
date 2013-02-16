#include "LEDController.h"
#include "Logging.h"

LEDController* LEDController::Instance = 0;

#ifdef RASPBERRY_PI
#include "wiringPi/wiringPi.h"
#endif

LEDController::LEDController(int clockPin, int dataPin)
	: m_clockPin(clockPin), m_dataPin(dataPin), m_isSetup(true)
{
	Instance = this;

	LOG_INFO("LEDController: setting up with ClockPin=" << clockPin << " DataPin=" << dataPin);

	#ifdef RASPBERRY_PI
	if (wiringPiSetup() < 0)
	{
		LOG_ERROR("LEDController: Failed to setup wiringPi");
		m_isSetup = false;
		return;
	}

	pinMode(m_clockPin, OUTPUT);
	pinMode(m_dataPin, OUTPUT);
	#endif
}

void LEDController::SetPins(int clockPin, int dataPin)
{
	m_clockPin = clockPin;
	m_dataPin = dataPin;

	#ifdef RASPBERRY_PI
	pinMode(m_clockPin, OUTPUT);
	pinMode(m_dataPin, OUTPUT);
	#endif
}

void LEDController::UpdateLeds(Color* colorBuffer, int numLeds)
{
	if (!m_isSetup)
		return;

	if (colorBuffer == 0)
		return;

	for (int i=0; i<numLeds; ++i)
	{
		Color color = colorBuffer[i];
		ShiftOut8Bits(color.R);
		ShiftOut8Bits(color.G);
		ShiftOut8Bits(color.B);
	}
	
	#ifdef RASPBERRY_PI
	digitalWrite(m_clockPin, 0);
	delay(1);
	#endif
}

void LEDController::ShiftOut8Bits(char c)
{
	#ifdef RASPBERRY_PI
	for (int bit = 0; bit < 8; bit++)
	{
		bool val = (c >> (7 - bit)) & 1;

		digitalWrite(m_clockPin, 0);
		digitalWrite(m_dataPin, val);
		digitalWrite(m_clockPin, 1);
	}
	#endif
}