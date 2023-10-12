// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#pragma once
#include "NetCodes/Session.h"
#include "NetCodes/Service.h"
#include "SToCSessionManager.h"

class SToLSession : public PacketSession
{
public:
	~SToLSession()
	{
		cout << "~SToLSession" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual int32 OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;
};