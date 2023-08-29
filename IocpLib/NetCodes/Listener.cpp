#include "../pch/pch.h"
#include "Listener.h"
#include "SocketModifier.h"
#include "IocpEvent.h"
#include "Session.h"
#include "Service.h"

Listener::~Listener()
{
	SocketModifier::Close(_socket);
	xdelete(_acceptEvent);
}

bool Listener::Init(SharedPtr<ServerService> service)
{
	_service = service;
	if (_service == nullptr)
		return false;

	_socket = SocketModifier::CreateSocket();
	if (_socket == INVALID_SOCKET)
		return false;
	if (_service->GetIocpCore()->Register(shared_from_this()) == false)
		return false;
	if (SocketModifier::SetReuseAddress(_socket, true) == false)
		return false;
	if (SocketModifier::SetLinger(_socket, 0, 0) == false)
		return false;
	if (SocketModifier::Bind(_socket, _service->GetNetAddress()) == false)
		return false;
	if (SocketModifier::Listen(_socket) == false)
		return false;

	_acceptEvent = xnew<AcceptEvent>();
	_acceptEvent->owner = shared_from_this();
	if (!_acceptEvent) return false;

	return true;
}

bool Listener::StartAccept()
{
	RegisterAccept(nullptr); // RegisterAccept 체인 최초 시작지점
	return true;
}

void Listener::CloseSocket()
{
	SocketModifier::Close(_socket);
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	ASSERT_CRASH(iocpEvent->eventType == EventType::Accept);
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept();
}

void Listener::RegisterAccept(SharedPtr<Session> unusedSession)
{
	// 세션 생성 혹은 재사용
	SharedPtr<Session> session = nullptr;
	if (unusedSession != nullptr) session = unusedSession;
	else session = _service->CreateSession();

	// 재사용 위해 기존 acceptEvent 정보 초기화
	_acceptEvent->Reset();
	_acceptEvent->session = session;

	// Service에 잔여 공간 생길 때까지 대기
	if (!_service->CanAddNewSession())
		cout << "[Listener] Service에 Connect할 수 있는 최대 세션 수가 가득 찼습니다. 잔여 공간이 생길 때까지 Accept 하지 않고 대기합니다." << endl;
	while (!_service->CanAddNewSession())
	{
	}

	DWORD bytesReceived = 0;
	if (false == SocketModifier::AcceptEx(_socket, session->GetSocket(), session->_recvBuffer.WritePos(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT & bytesReceived, static_cast<LPOVERLAPPED>(_acceptEvent)))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			cout << "[Listener] 에러가 발생했습니다 - 에러코드: " << errorCode << endl;
			RegisterAccept(unusedSession);
		}
	}
}

void Listener::ProcessAccept()
{
	SharedPtr<Session> session = _acceptEvent->session;

	if (false == SocketModifier::SetUpdateAcceptSocket(session->GetSocket(), _socket))
	{
		RegisterAccept(session); // 세션 처리 에러; 재사용
		return;
	}

	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
	{
		RegisterAccept(session); // 세션 처리 에러; 재사용
		return;
	}

	session->SetNetAddress(NetAddress(sockAddress));
	session->ProcessConnect();
	RegisterAccept(nullptr); // 세션 처리 성공; 새 세션 생성
}