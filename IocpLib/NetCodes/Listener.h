// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#pragma once
#include "IocpCore.h"
#include "NetAddress.h"
#include "Session.h"

class AcceptEvent;
class ServerService;

class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	bool Init(SharedPtr<ServerService> service);
	bool StartAccept();
	void CloseSocket();

public:
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	void RegisterAccept(SharedPtr<Session> unusedSession); // 세션 객체를 생성해 IOCP에 등록하고 acceptEvent를 대기한다; 한번 생성된 세션은 성공적으로 Connect될 때까지 재사용된다
	void ProcessAccept(); // _acceptEvent 정보를 기반으로 Accept를 처리한다

protected:
	SOCKET _socket = INVALID_SOCKET;
	AcceptEvent* _acceptEvent = nullptr; // 여러 스레드에서 사용되긴 하지만 한 번에 하나의 스레드에서만 접근하므로 락이 필요 없다
	SharedPtr<ServerService> _service = nullptr;
};

