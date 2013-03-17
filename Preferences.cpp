#include <fstream>
#include <streambuf>

#include "Preferences.h"
#include "Logging.h"

Preferences* Preferences::Instance = 0;

void Preferences::Load()
{
	LOG_INFO("Loading Preferences from file: ");

	// Load the file as json
	json_error_t error;
	json_t* root = json_load_file("prefs.txt", 0, &error);
	if (!root)
	{
		LOG_ERROR("Preferences.Load: error on line " << error.line << ":" << error.text);
		return;
	}

	// Read properties
	ReadVector2(root, "boundsTopLeft", BoundsTopLeft);
	ReadVector2(root, "boundsTopRight", BoundsTopRight);
	ReadVector2(root, "boundsBottomRight", BoundsBottomRight);
	ReadVector2(root, "boundsBottomLeft", BoundsBottomLeft);

	ReadInt(root, "totalFadeTimeMS", TotalFadeTimeMS);

	ReadBool(root, "fixedColorEnabled", FixedColorEnabled);
	ReadColor(root, "fixedColor", FixedColor);

	ReadInt8(root, "camBrightness", CamBrightness);
	ReadInt8(root, "camContrast", CamContrast);
	ReadInt8(root, "camSaturation", CamSaturation);
	ReadInt8(root, "camGain", CamGain);

	// Cleanup
	json_decref(root);

	// Call save once, this add new properties to the prefs file
	Save();
}

json_t* Preferences::JsonEncode() const
{
	// Create new empty json object
	json_t* root = json_object();

	// Add to root json object
	json_object_set_new(root, "boundsTopLeft", json_vector2(BoundsTopLeft));
	json_object_set_new(root, "boundsTopRight", json_vector2(BoundsTopRight));
	json_object_set_new(root, "boundsBottomRight", json_vector2(BoundsBottomRight));
	json_object_set_new(root, "boundsBottomLeft", json_vector2(BoundsBottomLeft));

	json_object_set_new(root, "totalFadeTimeMS", json_integer(TotalFadeTimeMS));

	json_object_set_new(root, "fixedColorEnabled", json_boolean(FixedColorEnabled));
	json_object_set_new(root, "fixedColor", json_color(FixedColor));

	json_object_set_new(root, "camBrightness", json_integer(CamBrightness));
	json_object_set_new(root, "camContrast", json_integer(CamContrast));
	json_object_set_new(root, "camSaturation", json_integer(CamSaturation));
	json_object_set_new(root, "camGain", json_integer(CamGain));

	return root;
}

void Preferences::Save() const
{
	LOG_INFO("Saving Preferences");

	// Write json to file
	json_t* root = JsonEncode();
	json_dump_file(root, "prefs.txt", 0);
	json_decref(root);
}

void Preferences::ReadVector2(const json_t *root, const char* propertyName, Vector2& dest)
{
	json_t* jVec2 = json_object_get(root, propertyName);
	if (!jVec2)
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' does not exist");
		return;
	}

	if (json_is_vector2(jVec2))
    {
		dest = json_vector2_value(jVec2);
		LOG_DEBUG(" - " << propertyName << " = (" << dest.X() << "," << dest.Y() << ")");
    }
	else
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' is not a vector2");
	}
}

void Preferences::ReadColor(const json_t *root, const char* propertyName, Color& dest)
{
	json_t* jColor = json_object_get(root, propertyName);
	if (!jColor)
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' does not exist");
		return;
	}

	if (json_is_color(jColor))
    {
		dest = json_color_value(jColor);
		LOG_DEBUG(" - " << propertyName << " = (" << dest.R << "," << dest.G << "," << dest.B << ")");
    }
	else
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' is not a color");
	}
}

void Preferences::ReadInt(const json_t *root, const char* propertyName, int& dest)
{
	json_t* jInt = json_object_get(root, propertyName);
	if (!jInt)
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' does not exist");
		return;
	}

	if (json_is_integer(jInt))
    {
		dest = (int)json_integer_value(jInt);
		LOG_DEBUG(" - " << propertyName << " = " << dest);
    }
	else
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' is not an integer");
	}
}

void Preferences::ReadInt8(const json_t *root, const char* propertyName, uint8_t& dest)
{
	int i;
	ReadInt(root, propertyName, i);
	dest = (uint8_t)i;
}

void Preferences::ReadFloat(const json_t *root, const char* propertyName, float& dest)
{
	json_t* jReal = json_object_get(root, propertyName);
	if (!jReal)
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' does not exist");
		return;
	}

	if (json_is_real(jReal))
    {
		dest = (float)json_real_value(jReal);
		LOG_DEBUG(" - " << propertyName << " = " << dest);
    }
	else
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' is not a float");
	}
}


void Preferences::ReadBool(const json_t *root, const char* propertyName, bool& dest)
{
	json_t* jBool = json_object_get(root, propertyName);
	if (!jBool)
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' does not exist");
		return;
	}

	if (json_is_boolean(jBool))
    {
		dest = json_is_true(jBool);
		LOG_DEBUG(" - " << propertyName << " = " << dest);
    }
	else
	{
		LOG_ERROR("Preferences.Load: '" << propertyName << "' is not a boolean");
	}
}
