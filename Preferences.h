#pragma once

#include <string>
#include <jansson.h>

#include "MathUtils.h"

using namespace std;

class Preferences
{
public:
	static Preferences* Instance;

	Preferences() : m_totalFadeTimeMS(200), 
					m_camBrightness(0.46f), m_camContrast(0.12f), m_camSaturation(0.29f), m_camGain(0.8f)
	{
		Instance = this;
	}

	void Load();
	json_t* JsonEncode() const;
	void Save() const;

	void GetBounds(Vector2& topLeft, Vector2& topRight, Vector2& bottomRight, Vector2& bottomLeft) const;
	void SetBounds(const Vector2& topLeft, const Vector2& topRight, const Vector2& bottomRight, const Vector2& bottomLeft);
	float GetTotalFadeTimeMS() { return m_totalFadeTimeMS; }

private:
	void ReadVector2(const json_t *root, const char* propertyName, Vector2& dest);
	void ReadInt(const json_t *root, const char* propertyName, int& dest);
	void ReadFloat(const json_t *root, const char* propertyName, float& dest);

	Vector2 m_boundsTopLeft;
	Vector2 m_boundsTopRight;
	Vector2 m_boundsBottomRight;
	Vector2 m_boundsBottomLeft;

	float m_totalFadeTimeMS;
	
	//int m_camExposure;
	float m_camBrightness;
	float m_camContrast;
	float m_camSaturation;
	float m_camGain;
};