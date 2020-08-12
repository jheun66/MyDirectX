#pragma once
// GlobalBuffer : ������۵�

class MatrixBuffer : public ConstBuffer
{
public:
	MatrixBuffer() : ConstBuffer(&data, sizeof(data))
	{
		data.matrix = XMMatrixIdentity();
	}

	void Set(XMMATRIX value)
	{
		data.matrix = XMMatrixTranspose(value);
	}

	struct Data
	{
		XMMATRIX matrix;
	};

private:
	Data data;

};

// ���� ��� ���۷� ���� : ���� ���� �߰��� �ٲ� �� �ֵ���
class ColorBuffer : public ConstBuffer
{
public:
	struct Data
	{
		XMFLOAT4 color;
	};

	ColorBuffer() : ConstBuffer(&data, sizeof(data))
	{
		data.color = { 1,1,1,1 };
	}

public:
	Data data;
};