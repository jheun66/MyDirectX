#pragma once

class RainMist : public Particle
{
private:
	VertexSize* vertices;

public:
	RainMist();
	~RainMist();

	// Particle��(��) ���� ��ӵ�
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;

};