#pragma once

class RainExplosion : public Particle
{
private:
	SparkBuffer* sparkBuffer;
	VertexParticle* vertices;
public:
	RainExplosion();
	~RainExplosion();

	// Particle��(��) ���� ��ӵ�
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	virtual void Play(Vector3 position) override;
};