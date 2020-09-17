#pragma once

class RainFall : public Particle
{
private:
	VertexSize* vertices;
	XMFLOAT3* endPos;

	Vector3 direction;

	Terrain* terrain = nullptr;

public:
	RainFall();
	~RainFall();

	// Particle��(��) ���� ��ӵ�
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;

	void PostRender();

	void SetTerrain(Terrain* terrain);
	function<void(Vector3)> Explosion;
};