#pragma once

class ShadowScene : public Scene
{
private:
	//ModelRender* plane;
	Terrain* terrain;
	Vanguard* vanguard;

	SettingBuffer* settingBuffer;

	Shadow* shadow;
public:
	ShadowScene();
	~ShadowScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};