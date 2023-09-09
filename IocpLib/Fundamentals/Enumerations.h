#pragma once
#include "../pch/pch.h"

enum HostTypeEnum : uint16
{
	NONE,
	CLIENT_HEADLESS,
	CLIENT,
	LOGIC_SERVER_HEADLESS,
	LOGIC_SERVER,
	MIDDLEMAN_SERVER,
};

enum ActionTypeEnum : uint32
{
	UNDEFINED,
	MOVE,
	LOOK,
	JUMP,
};

enum class ServiceType : uint8
{
	Middleman, // 다수의 클라이언트 세션들을 관리
	Client, // 하나의 미들맨과 연결된 클라이언트
	LogicServer // 하나의 미들맨과 연결된 서버
};
