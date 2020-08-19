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

// 접근성 편하게 할려고
public:
	string tag;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

protected:
	Vector3 pivot;

	XMMATRIX world;
	XMMATRIX* parent;

	MatrixBuffer* worldBuffer;

};