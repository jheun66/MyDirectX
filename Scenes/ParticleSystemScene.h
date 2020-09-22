#pragma once

class ParticleSystemScene : public Scene
{
private:
	Terrain* terrain;
	ParticleSystem* ps;

public:
	ParticleSystemScene();
	~ParticleSystemScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};