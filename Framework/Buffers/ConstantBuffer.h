#pragma once

class ConstBuffer
{
protected:
	ConstBuffer(void* data, UINT dataSize);
	virtual ~ConstBuffer();

	void MapData();

public:
	// shader ���� ����� �����
	void SetBufferToVS(UINT slot);
	void SetBufferToPS(UINT slot);

private:
	ID3D11Buffer* buffer;

	void* data;
	UINT dataSize;

};