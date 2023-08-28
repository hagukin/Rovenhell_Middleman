#include "../pch/pch.h"
#include "Listener.h"
#include "SocketModifier.h"
#include "IocpEvent.h"
#include "Session.h"
#include "Service.h"

Listener::~Listener()
{
	SocketModifier::Close(_socket);

	for (AcceptEvent* acceptEvent : _acceptEvents)
	{
		xdelete(acceptEvent);
	}
}

bool Listener::StartAccept(SharedPtr<ServerService> service)
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

	const int32 acceptCount = _service->GetMaxSessionCount();
	for (int32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = xnew<AcceptEvent>();
		acceptEvent->owner = shared_from_this();
		_acceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}

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
	ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	SharedPtr<Session> session = _service->CreateSession();

	acceptEvent->Init();
	acceptEvent->session = session;

	DWORD bytesReceived = 0;
	if (false == SocketModifier::AcceptEx(_socket, session->GetSocket(), session->_recvBuffer.WritePos(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			RegisterAccept(acceptEvent);
		}
	}
}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	SharedPtr<Session> session = acceptEvent->session;

	if (false == SocketModifier::SetUpdateAcceptSocket(session->GetSocket(), _socket))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	session->SetNetAddress(NetAddress(sockAddress));
	session->ProcessConnect();
	RegisterAccept(acceptEvent);
}