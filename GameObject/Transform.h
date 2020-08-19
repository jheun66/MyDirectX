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

// ���ټ� ���ϰ� �ҷ���
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