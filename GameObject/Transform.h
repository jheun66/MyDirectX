#pragma once

class Transform
{
public:
	Transform(string tag = "Untagged");
	virtual ~Transform();

	void UpdateWorld();

	void SetWorldBuffer(UINT slot = 0);

	XMMATRIX* GetWorld() { return &world; }
	void SetParent(XMMATRIX* value) { parent = value; }
	
	XMMATRIX* GetParent() { return parent; }

	Vector3 Forward();
	Vector3 Up();
	Vector3 Right();

	Vector3 WorldPos();	
	Vector3 WorldRot();
	Vector3 WorldScale();

// 접근성 편하게 할려고
public:
	string tag;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

protected:
	Vector3 globalPosition;
	Vector3 globalRotation;
	Vector3 globalScale;

	Vector3 pivot;

	XMMATRIX world;
	XMMATRIX* parent;

	MatrixBuffer* worldBuffer;
};