#pragma once
// GlobalBuffer : �پ��ѵ��� ����ϴ� ������۵�
// Ư���ѵ����� ����ϸ� �� Ŭ�������� ��������!

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

		// Exp ����
		float specularExp;
		// �ֺ��� ����
		XMFLOAT4 ambient;
		XMFLOAT4 ambientCeil;
	}data;

	DirLightBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.direction = { 0, -1, 0 };
		data.specularExp = 8;
		data.ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
		data.ambientCeil = { 0.1f, 0.1f, 0.1f, 1.0f };

	}

};

// ���� light ����
#define MAX_LIGHT 10
struct LightInfo
{
	enum Type
	{
		DIRECTION,
		POINT,
		SPOT,
		CAPSULE
	};

	XMFLOAT4 color;

	XMFLOAT3 position;
	float range;

	XMFLOAT3 direction;
	float outer;

	float inner;
	float length;
	Type type;
	int active;

	LightInfo()
	{
		color = XMFLOAT4(1, 1, 1, 1);
		position = XMFLOAT3(0, 0, 0);
		range = 80.0f;
		direction = XMFLOAT3(0, -1, 0);
		outer = 65.0f;
		inner = 55.0f;
		length = 50;
		type = POINT;
		active = 1;
	}

};

class LightInfoBuffer : public ConstBuffer
{
public:
	struct Data
	{
		LightInfo lights[MAX_LIGHT];

		UINT lightCount;
		float padding[3];
	}data;

	LightInfoBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.lightCount = 0;
	}

	void Add(LightInfo light)
	{
		data.lights[data.lightCount++] = light;
	}
	void Add()
	{
		data.lightCount++;
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

// ����(specular, normal) �� ���� ����
class MSBuffer : public ConstBuffer
{
public:
	struct Data
	{
		int option[4];

		Data() :option{}
		{}
	}data;

	MSBuffer() : ConstBuffer(&data, sizeof(Data))
	{
	}

};

class SizeBuffer : public ConstBuffer
{
public:
	struct Data
	{
		XMFLOAT2 size;
		XMFLOAT2 padding;

	}data;

	SizeBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.size = { 0, 0 };
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

// ������ ���������� �̸� ������
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
