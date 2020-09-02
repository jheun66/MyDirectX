#pragma once

class MoveModelScene : public Scene
{
public:
	MoveModelScene();
	~MoveModelScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void UpdateCamera();

private:
	Terrain* terrain;
	WarHawk* warHawk;

	MSBuffer* settingBuffer;

};