#pragma once

struct Ray
{
	Vector3 position;
	Vector3 direction;
};


class Camera : public Transform
{
public:
	Camera(string tag);
	~Camera();

	void Rotation();
	void Move();

	void Update();
	void PostRender();
	void CreatePerspective();
	void CreateView();

	XMMATRIX GetViewMatrix() { return matView; }
	XMMATRIX GetPerspectiveMatrix() { return matPerspective; }

	ViewBuffer* GetViewBuffer() { return viewBuffer; }
	MatrixBuffer* GetProjection() { return projectionBuffer; }

	Ray ScreenPointToRay(Vector3 pos);

private:	
	Vector3 forward;
	Vector3 right;
	Vector3 up;

	XMMATRIX matRotation;
	XMMATRIX matView;
	XMMATRIX matPerspective;

	ViewBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;

	POINT curPos;
	POINT oldPos;

	float FOV;

	float moveSpeed;
	float rotSpeed;
};