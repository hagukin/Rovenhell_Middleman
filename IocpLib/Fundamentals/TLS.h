#pragma once
#include <stack>

class SendBufferChunk;

extern thread_local uint32 LThreadId;
extern thread_local std::stack<int32> LLockStack;
extern thread_local SharedPtr<SendBufferChunk> LSendBufferChunk;