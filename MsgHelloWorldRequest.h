#pragma once

#include "MsgHandlerBase.h"
#include "MsgHelloWorldResponse.h"

class MsgHelloWorldRequest : public MsgHandlerBase
{
public:
	virtual bool Read(Connection* sender, NetIncomingMessage& msg)
	{
		if (!MsgHandlerBase::Read(sender, msg))
			return false;

		// m_message
		json_t* message = json_object_get(m_jsonRoot, "message");
        if (!json_is_string(message))
        {
            LOG_ERROR("MsgHelloWorldRequest.Read: " << "message is not a string");
            return false;
        }
		m_message = json_string_value(message);

		LOG_INFO("      -> " << m_message.c_str());

		// Send response
		MsgHelloWorldResponse* response = new MsgHelloWorldResponse("FUCK YEAH, response that shit!");
		sender->Send(response->Write());

		return true;
	}

private:
	string m_message;
};