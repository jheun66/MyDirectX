#pragma once

class ParticleSystemScene : public Scene
{
private:
	Terrain* terrain;
	ParticleSystem* ps;

public:
	ParticleSystemScene();
	~ParticleSystemScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};