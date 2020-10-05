#pragma once

class ModelScene : public Scene
{
public:
	ModelScene();
	~ModelScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	ModelReader* modelReader;
	ModelRender* model;
	SettingBuffer* settingBuffer;
};