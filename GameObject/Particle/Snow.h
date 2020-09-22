#pragma once

class Snow : public Particle
{
private:
	class SnowBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			XMFLOAT3 velocity;
			float distance;

			XMFLOAT4 color;

			XMFLOAT3 origin;
			float time;

			XMFLOAT3 size;
			float turbulence;
		}data;

		SnowBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.velocity = XMFLOAT3(0, -5, 0);
			data.distance = 50.0f;

			data.color = XMFLOAT4(1, 1, 1, 1);

			data.origin = XMFLOAT3(0, 0, 0);
			data.size = XMFLOAT3(10, 10, 10);

			data.time = 0.0f;
			data.turbulence = 5.0f;
		}
	};

	struct VertexSnow
	{
		XMFLOAT3 position;
		float scale;
		XMFLOAT2 random;
	};

	SnowBuffer* snowBuffer;
	VertexSnow* vertices;

public:
	Snow();
	~Snow();

	// Particle을(를) 통해 상속됨
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

};