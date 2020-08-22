#pragma once

class VertexBuffer
{
public:
	VertexBuffer(void* data, UINT stride, UINT count);
	~VertexBuffer();

	void Set();

private:
	ID3D11Buffer* buffer;

	UINT stride;
	UINT offset;
};