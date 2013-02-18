#pragma once

#include "MsgHandlerBase.h"
#include "MsgFrameBuffer.h"
#include "MsgPreferences.h"
#include "Logging.h"

class MsgGenericRequest : public MsgHandlerBase
{
public:
	virtual bool Read(Connection* sender, NetIncomingMessage& msg)
	{
		LOG_INFO("   MsgGenericRequest.Read: 1");

		if (msg.BodyLength() == 0)
			return false;

		// Read the body as json
		json_error_t error;
		json_t* root = json_loads(msg.Body(), 0, &error);
		if (!root)
		{
			LOG_ERROR("MsgGenericRequest.Read: error on line " << error.line << ":" << error.text);
			return false;
		}
		LOG_INFO("   MsgGenericRequest.Read: 2");

		// m_message
		json_t* request = json_object_get(root, "request");
		LOG_INFO("   MsgGenericRequest.Read: 5");
        if (!json_is_string(request))
        {
            LOG_ERROR("MsgGenericRequest.Read: 'request' is not a string");
            return false;
        }
		m_requestStr = json_string_value(request);
		LOG_INFO("   MsgGenericRequest.Read: 6");

		// Handle it yo!
		HandleRequest(sender);
		LOG_INFO("   MsgGenericRequest.Read: 7");

		// Cleanup
		json_decref(root);
		LOG_INFO("   MsgGenericRequest.Read: 8");

		return true;
	}

private:
	void HandleRequest(Connection* sender)
	{
		LOG_INFO(m_requestStr.c_str());

		if (m_requestStr == "getframebuffer")
		{
			LOG_INFO("MsgGenericRequest: handling 'getframebuffer' request");
			MsgFrameBuffer response;
			sender->Send(response.Write());
			return;
		}
		
		if (m_requestStr == "getprefs")
		{
			LOG_INFO("MsgGenericRequest: handling 'getprefs' request");
			MsgPreferences response;
			sender->Send(response.Write());
			return;
		}

		LOG_WARN("MsgGenericRequest: unhandled request '" << m_requestStr.c_str() << "'");
	}

	string m_requestStr;
};