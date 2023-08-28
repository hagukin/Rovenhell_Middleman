#pragma once
#include "IocpObject.h"

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE GetHandle() { return _iocpHandle; }

	bool Register(SharedPtr<IocpObject> iocpObject);
	bool Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE _iocpHandle;
};