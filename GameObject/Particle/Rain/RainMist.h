#pragma once

class RainMist : public Particle
{
private:
	VertexSize* vertices;

public:
	RainMist();
	~RainMist();

	// Particle을(를) 통해 상속됨
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;

};