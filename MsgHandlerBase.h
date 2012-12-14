#pragma once

#include "NetIncomingMessage.h"
#include "NetOutgoingMessage.h"
#include "NetMessageType.h"
#include "Connection.h"
#include "Logging.h"

class MsgHandlerBase
{
public:
	MsgHandlerBase()
	{
	}

	virtual ~MsgHandlerBase()
	{
	}

	virtual bool Read(Connection* sender, NetIncomingMessage& msg)
	{
		return true;
	}

	virtual NetOutgoingMessage* Write()
	{
		return new NetOutgoingMessage(MSG_INVALID);
	}
};