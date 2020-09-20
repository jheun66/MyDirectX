#pragma once
// GlobalBuffer : 다양한데서 사용하는 상수버퍼들
// 특정한데서만 사용하면 그 클래스에서 선언하자!

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
		data.ambient = { 0.8f, 0.8f, 0.8f, 0.0f };
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


class RayBuffer : public ConstBuffer
{
public:
	struct Data
	{
		XMFLOAT3 position;
		float size;
		XMFLOAT3 direction;
		float padding;
	}data;

	RayBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.position = XMFLOAT3(0, 0, 0);
		data.size = 0;
		data.direction = XMFLOAT3(0, 0, 0);
	}

};

class SparkBuffer :public ConstBuffer
{
public:
	struct Data
	{
		XMFLOAT3 direction;
		float duration;

		float time;
		float padding[3];

	}data;

	SparkBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.direction = { 0,0,0 };
		data.duration = 0.0f;
		data.time = 0.0f;
	}

};

// 위에와 동일하지만 이름 때문에
class FallBuffer : public ConstBuffer
{
public:
	struct Data
	{
		float duration;
		float time;
		float gravity;

		float padding;
	}data;

	FallBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.duration = 0.0f;
		data.time = 0.0f;
		data.gravity = 0.0f;
	}
};