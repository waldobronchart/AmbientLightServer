#pragma once

#include "MsgHandlerBase.h"
#include "Logging.h"
#include "MathUtils.h"
#include "Preferences.h"
#include "LEDController.h"

class MsgSetPins : public MsgHandlerBase
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
			LOG_ERROR("MsgSetBounds.Read: error on line " << error.line << ":" << error.text);
			return false;
		}

		// Read pins
		int clockPin, dataPin;
		json_t* pin;

		pin = json_object_get(root, "clockPin");
		if (json_is_integer(pin))
			clockPin = (int)json_integer_value(pin);

		pin = json_object_get(root, "dataPin");
		if (json_is_integer(pin))
			dataPin = (int)json_integer_value(pin);

		// Set pins
		Preferences::Instance->SetPins(clockPin, dataPin);
		LEDController::Instance->SetPins(clockPin, dataPin);

		// Cleanup
		json_decref(root);

		return true;
	}
};