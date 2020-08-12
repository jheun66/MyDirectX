#pragma once

class Transform
{
public:
	string tag;

	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

protected:
	XMFLOAT3 pivot;

	XMMATRIX world;
	XMMATRIX* parent;

	MatrixBuffer* worldBuffer;

public:
	Transform(string tag = "Untagged");
	virtual ~Transform();

	void UpdateWorld();

	void SetWorldBuffer(UINT slot = 0);

	XMMATRIX* GetWorld() { return &world; }
	void SetParent(XMMATRIX* value) { parent = value; }

};