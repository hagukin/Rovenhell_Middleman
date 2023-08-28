#pragma once
#include "NetCodes/Session.h"

class CToSSession : public PacketSession
{
public:
	~CToSSession();
	virtual void OnConnected() override;
	virtual int32 OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;
	virtual void OnDisconnected() override;
};