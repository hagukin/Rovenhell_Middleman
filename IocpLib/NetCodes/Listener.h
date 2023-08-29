#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

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
	void RegisterAccept(); // _acceptEvent를 초기화하고, 그 후 빈 Event 객체를 Register한다 (논블로킹)
	void ProcessAccept(); // _acceptEvent 정보를 기반으로 Accept를 처리한다

protected:
	SOCKET _socket = INVALID_SOCKET;
	AcceptEvent* _acceptEvent = nullptr; // 여러 스레드에서 사용되긴 하지만 한 번에 하나의 스레드에서만 접근하므로 락이 필요 없다
	SharedPtr<ServerService> _service = nullptr;
};

