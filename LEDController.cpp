#include "LEDController.h"
#include "Logging.h"
#include "Preferences.h"
#include "HardwareConfig.h"

#include <boost/timer.hpp>

LEDController* LEDController::Instance = 0;

// wiringPi only compiles on the RASPBERRY_PI
#ifdef RASPBERRY_PI
#include <wiringPi.h>
#endif

LEDController::LEDController() : m_isSetup(true), m_fadeTimeMS(200), m_prevColorBuffer(0)
{
	LOG_INFO("LEDController: setting up with ClockPin1=" << GPIO_CLOCK_PIN1 << " DataPin1=" << GPIO_DATA_PIN1
					<< " ClockPin2=" << GPIO_CLOCK_PIN2 << " DataPin2=" << GPIO_DATA_PIN2);

	Instance = this;

	// Prev color buffer for interpolation
	m_prevColorBuffer = new Color[TOTAL_NUM_LEDS];
	for (int i=0; i<TOTAL_NUM_LEDS; i++)
		m_prevColorBuffer[i] = Color(0, 0, 0);

	#ifdef RASPBERRY_PI
	if (wiringPiSetup() < 0)
	{
		LOG_ERROR("LEDController: Failed to setup wiringPi");
		m_isSetup = false;
		return;
	}

	pinMode(GPIO_CLOCK_PIN1, OUTPUT);
	pinMode(GPIO_CLOCK_PIN2, OUTPUT);

	pinMode(GPIO_DATA_PIN1, OUTPUT);
	pinMode(GPIO_DATA_PIN2, OUTPUT);
	#endif
}

void LEDController::UpdateLeds(Color* colorBuffer, float deltaTime)
{
	if (!m_isSetup)
		return;

	if (colorBuffer == 0)
		return;

	// This will update the led strands of 25 separately
	// I could have just attached the two strands together making it
	//  one big strand of 50, which uses just on data and one clock pin.
	// But unfortunately I was getting a lot of noise at the end of strand
	// That's why I split it up

	// Linear interpolation term for smoothness!
	float deltaTimeMS = deltaTime * 1000.0f;
	float fadeTimeMS = (float)m_fadeTimeMS;
	if (fadeTimeMS < deltaTimeMS)
		fadeTimeMS = deltaTimeMS;
	float lerpTerm = deltaTimeMS/fadeTimeMS;

	// Update first strand of 25
	for (int i=0; i<NUM_LEDS_PER_STRAND; ++i)
	{
		Color color = lerpColor(m_prevColorBuffer[i], colorBuffer[i], lerpTerm);

		ShiftOut8Bits(GPIO_CLOCK_PIN1, GPIO_DATA_PIN1, color.ByteR());
		ShiftOut8Bits(GPIO_CLOCK_PIN1, GPIO_DATA_PIN1, color.ByteG());
		ShiftOut8Bits(GPIO_CLOCK_PIN1, GPIO_DATA_PIN1, color.ByteB());

		m_prevColorBuffer[i] = color;
	}

	// Update second strand of 25
	for (int i=NUM_LEDS_PER_STRAND; i<TOTAL_NUM_LEDS; ++i)
	{
		Color color = lerpColor(m_prevColorBuffer[i], colorBuffer[i], lerpTerm);

		ShiftOut8Bits(GPIO_CLOCK_PIN2, GPIO_DATA_PIN2, color.ByteR());
		ShiftOut8Bits(GPIO_CLOCK_PIN2, GPIO_DATA_PIN2, color.ByteG());
		ShiftOut8Bits(GPIO_CLOCK_PIN2, GPIO_DATA_PIN2, color.ByteB());

		m_prevColorBuffer[i] = color;
	}
	
	#ifdef RASPBERRY_PI
	digitalWrite(GPIO_CLOCK_PIN1, 0);
	digitalWrite(GPIO_CLOCK_PIN2, 0);
	delay(1);
	#endif
}

void LEDController::UpdateLedsFixed(Color fixedColor, float deltaTime)
{
	// Update first strand of 25
	for (int i=0; i<NUM_LEDS_PER_STRAND; ++i)
	{
		ShiftOut8Bits(GPIO_CLOCK_PIN1, GPIO_DATA_PIN1, fixedColor.ByteR());
		ShiftOut8Bits(GPIO_CLOCK_PIN1, GPIO_DATA_PIN1, fixedColor.ByteG());
		ShiftOut8Bits(GPIO_CLOCK_PIN1, GPIO_DATA_PIN1, fixedColor.ByteB());
	}

	// Update second strand of 25
	for (int i=NUM_LEDS_PER_STRAND; i<TOTAL_NUM_LEDS; ++i)
	{
		ShiftOut8Bits(GPIO_CLOCK_PIN2, GPIO_DATA_PIN2, fixedColor.ByteR());
		ShiftOut8Bits(GPIO_CLOCK_PIN2, GPIO_DATA_PIN2, fixedColor.ByteG());
		ShiftOut8Bits(GPIO_CLOCK_PIN2, GPIO_DATA_PIN2, fixedColor.ByteB());
	}
	
	#ifdef RASPBERRY_PI
	digitalWrite(GPIO_CLOCK_PIN1, 0);
	digitalWrite(GPIO_CLOCK_PIN2, 0);
	delay(1);
	#endif
}

void LEDController::ShiftOut8Bits(int clockPin, int dataPin, uint8_t c)
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