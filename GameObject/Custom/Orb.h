#pragma once

// �׼��� �༺ ��� �����̴� ��ü
class Orb : public Sphere
{
public:
	Orb(float radius = 1.0f, UINT sliceCount = 50, UINT stackCount = 50);
	~Orb();

	void Update() override;
	void Render() override;

	void SetPivot(Vector3* value) { pivot = value; };
	void SetDistance(float value) { distance = value; }

	void SetRotSpeed(float value) { rotSpeed = value; }
	void SetRevSpeed(float value) { revSpeed = value; }

private:
	void Rotation();
	void Revolution();

private:
	// �߽�
	Vector3* pivot = nullptr;

	UINT distance = 0;
	float degree = 0;
	// ���� �ӵ�
	float rotSpeed = 20.0f;
	// ���� �ӵ�
	float revSpeed = 20.0f;
};