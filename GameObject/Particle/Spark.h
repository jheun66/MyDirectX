#pragma once

class Spark : public Particle
{
private:
	// 각각 GlobalBuffer, VertexLayouts 에 이동시킴

	SparkBuffer* sparkBuffer;
	VertexParticle* vertices;

public:
	Spark();
	~Spark();


	// Particle을(를) 통해 상속됨
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void Play(Vector3 position) override;
};