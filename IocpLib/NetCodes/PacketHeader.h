#pragma once
#include "../pch/pch.h"

// 상세 주석은 로직 서버 소스코드 참고

struct PacketHeader
{
	uint16 size;
	uint16 senderType;
	uint64 senderId;
	uint8 protocol;
	uint16 id;
	uint32 tick;
};

enum PacketId : uint16
{
	DEFAULT,
	CHAT_GLOBAL,
	ACTOR_PHYSICS,
	SESSION_INFO,
};

enum PacketProtocol : uint8
{
	NO_PROTOCOL,
	CLIENT_EVENT_ON_RECV,
	CLIENT_EVENT_ON_TICK_STRICT,
	CLIENT_EVENT_ON_TICK_LOOSE,
	LOGIC_EVENT,
	MIDDLEMAN_EVENT,
};