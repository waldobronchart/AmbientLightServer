#pragma once

#include "MsgHandlerBase.h"
#include "MsgFrameBuffer.h"
#include "Logging.h"
#include "MathUtils.h"
#include "Preferences.h"
#include "TrapezoidSampler.h"

class MsgSetBounds : public MsgHandlerBase
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

		// Read bounds
		Vector2 topLeft, topRight, bottomRight, bottomLeft;
		json_t* vec;

		vec = json_object_get(root, "boundsTopLeft");
		if (json_is_vector2(vec))
			topLeft = json_vector2_value(vec);

		vec = json_object_get(root, "boundsTopRight");
		if (json_is_vector2(vec))
			topRight = json_vector2_value(vec);

		vec = json_object_get(root, "boundsBottomRight");
		if (json_is_vector2(vec))
			bottomRight = json_vector2_value(vec);

		vec = json_object_get(root, "boundsBottomLeft");
		if (json_is_vector2(vec))
			bottomLeft = json_vector2_value(vec);

		// Set bounds
		Preferences::Instance->BoundsTopLeft = topLeft;
		Preferences::Instance->BoundsTopRight = topRight;
		Preferences::Instance->BoundsBottomRight = bottomRight;
		Preferences::Instance->BoundsBottomLeft = bottomLeft;
		TrapezoidSampler::Instance->UpdatePoints(topLeft, topRight, bottomRight, bottomLeft);

		// Cleanup
		json_decref(root);

		return true;
	}
};