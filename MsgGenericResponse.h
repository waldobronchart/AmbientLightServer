#pragma once

#include "MsgHandlerBase.h"
#include "NetMessageType.h"
#include <cstring>

class MsgGenericResponse : public MsgHandlerBase
{
public:
	static NetOutgoingMessage* WriteErrorResponse(string msg)
	{
		MsgGenericResponse r;
		r.m_sevirity = "error";
		r.m_msg = msg;
		return r.Write();
	}

	static NetOutgoingMessage* WriteInfoResponse(string msg)
	{
		MsgGenericResponse r;
		r.m_sevirity = "info";
		r.m_msg = msg;
		return r.Write();
	}

	static NetOutgoingMessage* WriteWarningResponse(string msg)
	{
		MsgGenericResponse r;
		r.m_sevirity = "warning";
		r.m_msg = msg;
		return r.Write();
	}

	virtual NetOutgoingMessage* Write()
	{
		// Create new empty json object
		json_t* root = json_object();

		// Values to json types
		json_t* jsonResponse = json_string(m_msg.c_str());

		// Add to root json object
		json_object_set(root, m_sevirity.c_str(), jsonResponse);
		
		// Create outgoing message
		NetOutgoingMessage* msg = new NetOutgoingMessage(MSG_GENERIC_REPONSE);
		char* dataStr = json_dumps(root, 0);
		msg->SetData(dataStr);
		delete dataStr; dataStr = 0; delete[] dataStr;

		// Cleanup
		json_decref(jsonResponse);
		json_decref(root);

		return msg;
	}

private:
	string m_sevirity;
	string m_msg;
};