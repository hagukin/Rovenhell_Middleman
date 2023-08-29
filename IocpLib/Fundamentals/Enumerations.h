#pragma once
#include "../pch/pch.h"

enum HostTypeEnum : uint8
{
	NONE,
	CLIENT_HEADLESS,
	CLIENT,
	LOGIC_SERVER_HEADLESS,
	LOGIC_SERVER,
	MIDDLEMAN_SERVER,
};

enum class ServiceType : uint8
{
	Middleman, // 다수의 클라이언트 세션들을 관리
	Client, // 하나의 미들맨과 연결된 클라이언트
	LogicServer // 하나의 미들맨과 연결된 서버
};
