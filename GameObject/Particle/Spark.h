#pragma once

class Spark : public Particle
{
private:
	// ���� GlobalBuffer, VertexLayouts �� �̵���Ŵ

	SparkBuffer* sparkBuffer;
	VertexParticle* vertices;

public:
	Spark();
	~Spark();


	// Particle��(��) ���� ��ӵ�
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void Play(Vector3 position) override;
};