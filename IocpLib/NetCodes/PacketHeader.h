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
};

enum PacketId : uint16
{
	DEFAULT,
	GAME_INPUT,
	CHAT_GLOBAL,
	ACTOR_PHYSICS,
	GAME_STATE,
	SESSION_INFO,
};

enum PacketProtocol : uint8
{
	NO_PROTOCOL,
	CLIENT_ONCE_PER_TICK,
	CLIENT_ALLOW_MULTIPLE_PER_TICK,
	LOGIC_EVENT,
	MIDDLEMAN_EVENT,
};