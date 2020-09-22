#pragma once

class ParticleSystem : public Particle
{
private:
	class TimeBuffer :public ConstBuffer
	{
	public:
		struct Data
		{
			float delta;
			float gravity;
			UINT outputSize;    // 1000개
			float padding;
		}data;

		TimeBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.delta = Time::Delta();
			data.gravity = 0.0f;
			data.outputSize = 1000;
		}
	};


	//struct Shape
	//{
	//	// 초기값들
	//	int shapeType = 0;
	//	XMFLOAT3 position = { 0, 0, 0 };
	//	XMFLOAT3 rotation = { 0, 0, 0 };
	//	XMFLOAT3 scale = { 1, 1, 1 };

	//	float radius = 1.0f;
	//	float angle = 30.0f;
	//};
	typedef VertexParticle VertexType;

	VertexType* vertices;
	TimeBuffer* timeBuffer;
	float duration = 5.0f;
	float lifeTime = 5.0f;
	bool looping = false;

	//Shape* shape;

	// 시간당 몇개를 내보낼지 (역수를 이용해서 시간당 한개씩 보내기)
	float rateOverTime = 5.0f;
	float rateTime = 0.0f;
	// 누적 시간
	float accumulatedTime = 0.0f;

	UINT emitIndex = 0;

	ComputeShader* computeShader;
	StructuredBuffer* structuredBuffer;
	VertexType* input;
	VertexType* output;
public:
	ParticleSystem();
	~ParticleSystem();

	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	virtual void Play(Vector3 position) override;

	void ChangeDiffuseMap(wstring diffuseFile);

	void CreateCompute();
	void ComputeParticle();

	void EmitParticle();
};