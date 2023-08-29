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
	RegisterAccept(); // RegisterAccept 체인 최초 시작지점
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

void Listener::RegisterAccept()
{
	// 재활용 위해 기존 acceptEvent 정보 초기화
	_acceptEvent->Reset();
	SharedPtr<Session> session = _service->CreateSession();
	_acceptEvent->session = session;

	DWORD bytesReceived = 0;
	if (false == SocketModifier::AcceptEx(_socket, session->GetSocket(), session->_recvBuffer.WritePos(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT & bytesReceived, static_cast<LPOVERLAPPED>(_acceptEvent)))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			RegisterAccept();
		}
	}
}

void Listener::ProcessAccept()
{
	SharedPtr<Session> session = _acceptEvent->session;

	if (false == SocketModifier::SetUpdateAcceptSocket(session->GetSocket(), _socket))
	{
		RegisterAccept();
		return;
	}

	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
	{
		RegisterAccept();
		return;
	}

	session->SetNetAddress(NetAddress(sockAddress));
	session->ProcessConnect();
	RegisterAccept();
}