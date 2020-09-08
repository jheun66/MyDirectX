#pragma once

class FreeCamera;
class FollowCamera;

class Camera : public Transform
{
protected:
	Camera();
	virtual ~Camera();

public:
	static Camera* Get() { return instance; }
	static void Create(); 
	static void Delete() { delete instance; }

	virtual void Update();
	virtual void PostRender();

	// 자식 클래스에서
	virtual void Move() {}
	virtual void Rotation() {}
	virtual void SetTarget(Transform* value) {}

	void CreatePerspective();
	void CreateView();

	XMMATRIX GetViewMatrix() { return matView; }
	XMMATRIX GetPerspectiveMatrix() { return matPerspective; }

	ViewBuffer* GetViewBuffer() { return viewBuffer; }
	MatrixBuffer* GetProjection() { return projectionBuffer; }

	Ray ScreenPointToRay(Vector3 pos);

protected:
	static Camera* instance;

	XMMATRIX matView;
	XMMATRIX matPerspective;

	ViewBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;

	// 전체 모니터 화면에서의 위치
	POINT curPos;
	POINT oldPos;

	float FOV;
};