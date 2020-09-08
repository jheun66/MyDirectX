#pragma once

class FollowCamera : public Camera
{
	friend class Camera;
public:
	FollowCamera();
	~FollowCamera();

	void Update() override;
	void PostRender() override;

	void Move() override;
	// 1��Ī
	void FPMove();

	void SetTarget(Transform* value) override { target = value; }

	void MouseControl();

private:
	// xz�� ����
	float distance;
	// y�� ����
	float height;

	XMMATRIX matRotation;

	Vector3 offset;

	Vector3 destPos;
	float destRot;

	float moveDamping;
	float rotDamping;

	float rotY;
	float rotSpeed;

	float zoomSpeed;

	Transform* target;

	// 1��Ī
	bool isFP;
};