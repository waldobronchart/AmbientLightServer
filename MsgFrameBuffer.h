#pragma once

#include "MsgHandlerBase.h"
#include "NetMessageType.h"
#include "MsgGenericResponse.h"
#include "CameraController.h"
#include <cstring>

class MsgFrameBuffer : public MsgHandlerBase
{
public:
	virtual NetOutgoingMessage* Write()
	{
		const IplImage* frame = CameraController::Instance->Frame();

		// If frame is null, just send a generic error response
		if (frame == 0)
		{
			MsgGenericResponse msg;
			return msg.WriteErrorResponse("frame was requested but frame is null.");
		}

		// Create new empty json object
		json_t* root = json_object();

		// Encode RGB values into 32bit int (we only use 8bit per channel, so 24bits total)
		json_t* pixelBufferArray = json_array();
		char* frameBuffer = frame->imageData;
		int numPixels = frame->width * frame->height;
		for (int pixel=0; pixel<numPixels; pixel++)
		{
			unsigned int pixelData = 0;
			for (int channel=0; channel<3; channel++)
			{
				char channelData = frameBuffer[(pixel*3)+channel];
				pixelData |= channelData << (8*channel);
			}

			json_t* jsonInt = json_integer(pixelData);
			json_array_append(pixelBufferArray, jsonInt);
			json_decref(jsonInt);
		}

		// Add to root json object
		json_object_set_new(root, "width", json_integer(frame->width));
		json_object_set_new(root, "height", json_integer(frame->height));
		json_object_set_new(root, "imageSize", json_integer(frame->imageSize));
		json_object_set_new(root, "channelSeq", json_string(frame->channelSeq));
		json_object_set_new(root, "imageData", pixelBufferArray);
		json_object_set_new(root, "colorModel", json_string(frame->colorModel));
		
		// Create outgoing message
		NetOutgoingMessage* msg = new NetOutgoingMessage(MSG_FRAME_BUFFER);
		char* dataStr = json_dumps(root, 0);
		msg->SetData(dataStr);
		delete dataStr; dataStr = 0; delete[] dataStr;

		// Cleanup
		json_decref(root);

		return msg;
	}
};