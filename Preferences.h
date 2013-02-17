#pragma once

#include <string>
#include <jansson.h>

#include "MathUtils.h"

using namespace std;

class Preferences
{
public:
	static Preferences* Instance;

	Preferences() :
	  m_numLedsHorizontal(16), m_numLedsVertical(9), m_clockPin(0), m_dataPin(1)
	{
		Instance = this;
	}

	void Load();
	json_t* JsonEncode() const;
	void Save() const;

	void GetBounds(Vector2& topLeft, Vector2& topRight, Vector2& bottomRight, Vector2& bottomLeft) const;
	void SetBounds(const Vector2& topLeft, const Vector2& topRight, const Vector2& bottomRight, const Vector2& bottomLeft);
	void GetPins(int& clockPin, int& dataPin);
	void SetPins(int clockPin, int dataPin);

private:
	void ReadVector2(const json_t *root, const char* propertyName, Vector2& dest);
	void ReadInt(const json_t *root, const char* propertyName, int& dest);

	Vector2 m_boundsTopLeft;
	Vector2 m_boundsTopRight;
	Vector2 m_boundsBottomRight;
	Vector2 m_boundsBottomLeft;

	int m_numLedsHorizontal;
	int m_numLedsVertical;

	// todo: take out pins from prefs
	int m_clockPin;
	int m_dataPin;
	
	/*int m_capPropExposure;
	unsigned char m_capPropBrightness;
	unsigned char m_capPropContrast;
	unsigned char m_capPropGain;
	unsigned char m_capPropSaturation;*/
};