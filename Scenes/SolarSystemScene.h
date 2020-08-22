#pragma once

class SolarSystemScene : public Scene
{
private:
	Orb* sun;
	Orb* earth;
	Orb* moon;

	Orb* mercury;
	Orb* venus;
	Orb* mars;
	Orb* jupiter;

	ColorBuffer* colorBuffer;
	PointLightBuffer* lightBuffer;

public:
	SolarSystemScene();
	~SolarSystemScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};
