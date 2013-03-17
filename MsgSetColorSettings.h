#pragma once

#include "MsgHandlerBase.h"
#include "MsgFrameBuffer.h"
#include "Logging.h"
#include "MathUtils.h"
#include "Preferences.h"
#include "TrapezoidSampler.h"

class MsgSetColorSettings : public MsgHandlerBase
{
public:
	virtual bool Read(Connection* sender, NetIncomingMessage& msg)
	{
		if (msg.BodyLength() == 0)
			return false;

		// Read the body as json
		json_error_t error;
		json_t* root = json_loads(msg.Body(), 0, &error);
		if (!root)
		{
			LOG_ERROR("MsgSetColorSettings.Read: error on line " << error.line << ":" << error.text);
			return false;
		}

		Preferences* prefs = Preferences::Instance;

		// TotalFadeTimeMS
		json_t* totalFadeTimeMS = json_object_get(root, "totalFadeTimeMS");
		if (json_is_integer(totalFadeTimeMS))
			prefs->TotalFadeTimeMS = (int)json_integer_value(totalFadeTimeMS);

		// Fixed Color settings
		json_t* fixedColorEnabled = json_object_get(root, "fixedColorEnabled");
		if (json_is_boolean(fixedColorEnabled))
			prefs->FixedColorEnabled = json_is_true(fixedColorEnabled);

		json_t* fixedColor = json_object_get(root, "fixedColor");
		if (json_is_color(fixedColor))
			prefs->FixedColor = json_color_value(fixedColor);

		// Camera settings
		json_t* camSaturation = json_object_get(root, "camSaturation");
		if (json_is_integer(camSaturation))
			prefs->CamSaturation = (uint8_t)json_integer_value(camSaturation);

		json_t* camBrightness = json_object_get(root, "camBrightness");
		if (json_is_integer(camBrightness))
			prefs->CamBrightness = (uint8_t)json_integer_value(camBrightness);

		json_t* camContrast = json_object_get(root, "camContrast");
		if (json_is_integer(camContrast))
			prefs->CamContrast = (uint8_t)json_integer_value(camContrast);

		json_t* camGain = json_object_get(root, "camGain");
		if (json_is_integer(camGain))
			prefs->CamGain = (uint8_t)json_integer_value(camGain);

		// Update camera
		CameraController::Instance->UpdateSettings(prefs->CamSaturation, prefs->CamBrightness, prefs->CamContrast, prefs->CamGain);

		// Cleanup
		json_decref(root);

		return true;
	}
};