// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#pragma once

class SendBufferChunk;

class SendBuffer
{
public:
	SendBuffer(SharedPtr<SendBufferChunk> owner, BYTE* buffer, int32 allocSize);
	~SendBuffer();

	BYTE* Buffer() { return _buffer; }
	int32 WriteSize() { return _writeSize; }
	void Close(uint32 writeSize);

private:
	BYTE* _buffer;
	uint32 _allocSize = 0;
	uint32 _writeSize = 0;
	SharedPtr<SendBufferChunk> _owner;
};


class SendBufferChunk : public enable_shared_from_this<SendBufferChunk>
{
	enum
	{
		SEND_BUFFER_CHUNK_SIZE = 6000
	};

public:
	SendBufferChunk();
	~SendBufferChunk();

	void Reset();
	SharedPtr<SendBuffer> Open(uint32 allocSize);
	void Close(uint32 writeSize);

	bool IsOpen() { return _open; }
	BYTE* Buffer() { return &_buffer[_usedSize]; }
	uint32 FreeSize() { return static_cast<uint32>(_buffer.size()) - _usedSize; }

private:
	Array<BYTE, SEND_BUFFER_CHUNK_SIZE> _buffer = {};
	bool _open = false;
	uint32 _usedSize = 0;
};


class SendBufferManager
{
public:
	SharedPtr<SendBuffer> Open(uint32 size);

private:
	SharedPtr<SendBufferChunk> Pop();
	void Push(SharedPtr<SendBufferChunk> buffer);

	static void PushGlobal(SendBufferChunk* buffer);

private:
	USE_LOCK;
	Vector<SharedPtr<SendBufferChunk>> _sendBufferChunks;
};
