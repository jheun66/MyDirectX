#pragma once

class ConstBuffer
{
protected:
	ConstBuffer(void* data, UINT dataSize);
	virtual ~ConstBuffer();

	void MapData();

public:
	// shader 마다 만들어 줘야함
	void SetVSBuffer(UINT slot);
	void SetPSBuffer(UINT slot);
	void SetCSBuffer(UINT slot);

private:
	ID3D11Buffer* buffer;

	void* data;
	UINT dataSize;

};