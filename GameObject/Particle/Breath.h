#pragma once

class Breath : public Particle
{
	struct VertexParticle
	{
		XMFLOAT3 position;
		XMFLOAT2 size;
		XMFLOAT3 velocity;
		float startTime;

		VertexParticle()
			:position(0, 0, 0), size(0, 0), velocity(0, 0, 0), startTime(0)
		{
		}
	};

	class BreathBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			XMFLOAT3 direction;
			float lifeTime;

			float time;
			float padding[3];
		}data;

		BreathBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.lifeTime = 0.0f;
			data.time = 0.0f;
			data.direction = { 0, 0, 0 };
		}
	};

	BreathBuffer* breathBuffer;
	VertexParticle* vertices;

	float minSpeed;
	float maxSpeed;

	XMFLOAT2 minSize;
	XMFLOAT2 maxSize;

	float maxStartTime;
public:
	Breath();
	~Breath();

	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	virtual void Play(Vector3 position, Vector3 rotation) override;

	void UpdateParticle();
};