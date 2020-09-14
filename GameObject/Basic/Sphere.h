#pragma once

//class Sphere : public Transform
//{
//	typedef VertexUVNormalTangent VertexType;
//public:
//	Sphere(float radius = 1.0f, UINT sliceCount = 50, UINT stackCount = 50);
//	virtual ~Sphere();
//
//	virtual void Update();
//	virtual void Render();
//
//	// �ܺο��� �ؽ��� ������ ����
//	Material* GetMaterial() { return material; }
//
//	// ���׸��� �ٲ��ֱ�
//	void SetMaterial(wstring shader) { delete material; material = new Material(shader); }
//
//private:
//	void CreateData();
//	void CreateTangent();
//
//protected:
//	Mesh* mesh;
//	Material* material;
//
//	vector<VertexType> vertices;
//	vector<UINT> indices;
//
//	UINT sliceCount;
//	UINT stackCount;
//	float radius;
//
//	RasterizerState* fillMode[2];
//};

class Sphere : public Transform
{
public:
	Sphere(float radius = 1.0f, UINT sliceCount = 20, UINT stackCount = 20);
	Sphere(wstring shaderFile, float radius = 1.0f, UINT sliceCount = 20, UINT stackCount = 20);
	~Sphere();

	void Update();
	void Render();

	float GetRadius() { return radius; }
	Material* GetMaterial() { return material; }
	RasterizerState* GetFillMode() { return fillMode[1]; }

private:
	void CreateMesh();
	void CreateTangent();

private:
	typedef VertexUVNormalTangent VertexType;

	// Material
	Material* material;

	// Mesh
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT sliceCount;
	UINT stackCount;
	float radius;

	RasterizerState* fillMode[2];
};