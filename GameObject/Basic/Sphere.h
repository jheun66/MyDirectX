#pragma once

class Sphere : public Transform
{
	typedef VertexUVNormalTangent VertexType;
public:
	Sphere(float radius = 1.0f, UINT sliceCount = 50, UINT stackCount = 50);
	virtual ~Sphere();

	virtual void Update();
	virtual void Render();

	// 외부에서 텍스쳐 입히기 위해
	Material* GetMaterial() { return material; }

	// 머테리얼 바꿔주기
	void SetMaterial(wstring shader) { delete material; material = new Material(shader); }

private:
	void CreateData();
	void CreateTangent();

protected:
	Mesh* mesh;
	Material* material;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT sliceCount;
	UINT stackCount;
	float radius;

	RasterizerState* fillMode[2];
};