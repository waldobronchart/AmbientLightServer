#pragma once

#include "MsgHandlerBase.h"
#include "NetMessageType.h"
#include "Preferences.h"
#include <cstring>

class MsgPreferences : public MsgHandlerBase
{
public:
	virtual NetOutgoingMessage* Write()
	{
		json_t* root = Preferences::Instance->JsonEncode();
		char* dataStr = json_dumps(root, 0);
		
		// Create outgoing message
		NetOutgoingMessage* msg = new NetOutgoingMessage(MSG_PREFERENCES);
		msg->SetData(dataStr);

		// Cleanup
		delete dataStr; dataStr = 0; delete[] dataStr;
		json_decref(root);

		return msg;
	}
};