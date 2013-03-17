#pragma once

#include <stdint.h>
#include <string>
#include <jansson.h>

#include "Color.h"
#include "MathUtils.h"

using namespace std;

class Preferences
{
public:
	static Preferences* Instance;

	Preferences() : TotalFadeTimeMS(200), 
					BoundsTopLeft(0.2f, 0.2f), BoundsTopRight(0.8f, 0.2f), BoundsBottomRight(0.8f, 0.8f), BoundsBottomLeft(0.2f, 0.8f),
					FixedColorEnabled(false), FixedColor(255, 0, 0),
					CamSaturation(117), CamBrightness(30), CamContrast(74), CamGain(204)
	{
		Instance = this;
	}

	void Load();
	json_t* JsonEncode() const;
	void Save() const;

public:
	// You know what, fuck getters and setters. What a waste of time

	// Bounds coordinates in normalized coordinates ([0, 1], [0, 1])
	Vector2 BoundsTopLeft;		
	Vector2 BoundsTopRight;
	Vector2 BoundsBottomRight;
	Vector2 BoundsBottomLeft;

	// Fade time; makes color transitions smoother
	int TotalFadeTimeMS;

	// Fixed color settings
	bool FixedColorEnabled;
	Color FixedColor;
	
	// Camera settings
	uint8_t CamSaturation;
	uint8_t CamBrightness;
	uint8_t CamContrast;
	uint8_t CamGain;

private:
	void ReadVector2(const json_t *root, const char* propertyName, Vector2& dest);
	void ReadColor(const json_t *root, const char* propertyName, Color& dest);
	void ReadInt(const json_t *root, const char* propertyName, int& dest);
	void ReadInt8(const json_t *root, const char* propertyName, uint8_t& dest);
	void ReadFloat(const json_t *root, const char* propertyName, float& dest);
	void ReadBool(const json_t *root, const char* propertyName, bool& dest);
};