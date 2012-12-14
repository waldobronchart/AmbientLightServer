#pragma once

#include "MsgHandlerBase.h"

class MsgHelloWorldResponse : public MsgHandlerBase
{
public:
	MsgHelloWorldResponse(string message) : m_message(message)
	{
	
	}

	virtual NetOutgoingMessage* Write()
	{
		// Create new empty json object
		json_t* root = json_object();

		// Write values to json
		json_object_set(root, "message", json_string(m_message.c_str()));

		// Create outgoing message
		NetOutgoingMessage* msg = new NetOutgoingMessage(MSG_FRAME_RESPONSE);
		msg->SetData(json_dumps(root, 0));

		// Cleanup
		delete root;

		return msg;
	}

private:
	string m_message;
};