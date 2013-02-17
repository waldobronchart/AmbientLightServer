#include "LEDController.h"
#include "Logging.h"
#include "Preferences.h"

#include <boost/timer.hpp>

LEDController* LEDController::Instance = 0;

#ifdef RASPBERRY_PI
#include "wiringPi/wiringPi.h"
#endif

LEDController::LEDController()
	: m_clockPin1(0), m_clockPin2(3), m_dataPin1(1), m_dataPin2(4), m_isSetup(true)
{
	Instance = this;

	LOG_INFO("LEDController: setting up with ClockPin1=" << m_clockPin1 << " DataPin1=" << m_dataPin1
					<< " ClockPin2=" << m_clockPin2 << " DataPin2=" << m_dataPin2);

	#ifdef RASPBERRY_PI
	if (wiringPiSetup() < 0)
	{
		LOG_ERROR("LEDController: Failed to setup wiringPi");
		m_isSetup = false;
		return;
	}

	pinMode(m_clockPin1, OUTPUT);
	pinMode(m_clockPin2, OUTPUT);

	pinMode(m_dataPin1, OUTPUT);
	pinMode(m_dataPin2, OUTPUT);
	#endif
}

void LEDController::UpdateLeds(Color* colorBuffer, int numLeds)
{
	if (!m_isSetup)
		return;

	if (colorBuffer == 0)
		return;

	// I could have just attached the two strands together
	//  making it one big strand of 50, which uses just on data and one clock pin
	// But unfortunately I was getting a lot of noise at the end of strand
	// That's why I split it up
	int ledsPerStrand = numLeds / 2;

	// Update first strand of 25
	for (int i=0; i<ledsPerStrand; ++i)
	{
		Color color = colorBuffer[i];
		ShiftOut8Bits(m_clockPin1, m_dataPin1, color.R);
		ShiftOut8Bits(m_clockPin1, m_dataPin1, color.G);
		ShiftOut8Bits(m_clockPin1, m_dataPin1, color.B);
	}

	// Update second strand of 25
	for (int i=ledsPerStrand; i<numLeds; ++i)
	{
		Color color = colorBuffer[i];
		ShiftOut8Bits(m_clockPin2, m_dataPin2, color.R);
		ShiftOut8Bits(m_clockPin2, m_dataPin2, color.G);
		ShiftOut8Bits(m_clockPin2, m_dataPin2, color.B);
	}
	
	#ifdef RASPBERRY_PI
	digitalWrite(m_clockPin1, 0);
	digitalWrite(m_clockPin2, 0);
	delay(1);
	#endif
}

void LEDController::ShiftOut8Bits(int clockPin, int dataPin, char c)
{
	#ifdef RASPBERRY_PI
	for (int bit = 0; bit < 8; bit++)
	{
		bool val = (c >> (7 - bit)) & 1;

		digitalWrite(clockPin, 0);
		digitalWrite(dataPin, val);
		digitalWrite(clockPin, 1);
	}
	#endif
}