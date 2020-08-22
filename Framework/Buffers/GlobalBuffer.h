#pragma once
// GlobalBuffer : 상수버퍼들

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

// 색을 상수 버퍼로 전달 : 색을 게임 중간에 바꿀 수 있도록
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

class PointLightBuffer : public ConstBuffer
{
public:
	struct Data
	{
		XMFLOAT3 position;
		float padding;
	}data;

	PointLightBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.position = { 0,0,0 };
	}
};

class DirLightBuffer : public ConstBuffer
{
public:
	struct Data
	{
		XMFLOAT3 direction;

		// Exp 지수
		float specularExp;
		// 주변광 비율
		XMFLOAT4 ambient;
	}data;

	DirLightBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.direction = { 0, -1, 0 };
		data.specularExp = 8;
		data.ambient = { 0.1f, 0.1f, 0.1f, 0.0f };
	}

};


class ViewBuffer : public ConstBuffer
{
private:
	struct Data
	{
		XMMATRIX view;
		XMMATRIX invView;
	}data;

public:
	ViewBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.view = XMMatrixIdentity();
		data.invView = XMMatrixIdentity();
	}

	void Set(XMMATRIX value)
	{
		data.view = XMMatrixTranspose(value);
		XMMATRIX tmp = XMMatrixInverse(nullptr, value);
		data.invView = XMMatrixTranspose(tmp);
	}

};

// 여러(specular, normal) 맵 설정 적용
class MSBuffer : public ConstBuffer
{
public:
	struct Data
	{
		int option[16];

		// 현재까지 들어간 맵은 specular, normal, 초기값을 1로 함
		Data() :option{1,1}
		{}
	}data;

	MSBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}
};