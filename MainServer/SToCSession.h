#pragma once
#include "NetCodes/Session.h"

class SToCSession : public PacketSession
{
public:
	~SToCSession()
	{
		cout << "~SToCSession" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual int32 OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;
};