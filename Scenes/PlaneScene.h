#pragma once
class PlaneScene : public Scene
{
public:
	PlaneScene();
	~PlaneScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	Plane* plane = nullptr;
};