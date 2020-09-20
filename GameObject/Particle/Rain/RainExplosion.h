#pragma once

class RainExplosion : public Particle
{
private:
	SparkBuffer* sparkBuffer;
	VertexParticle* vertices;
public:
	RainExplosion();
	~RainExplosion();

	// Particle을(를) 통해 상속됨
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	virtual void Play(Vector3 position) override;
};