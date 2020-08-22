#pragma once

class Sphere : public Transform
{
	typedef VertexUVNormalTangent VertexType;
public:
	Sphere(float radius = 1.0f, UINT sliceCount = 50, UINT stackCount = 50);
	virtual ~Sphere();

	virtual void Update();
	virtual void Render();

	// �ܺο��� �ؽ��� ������ ����
	Material* GetMaterial() { return material; }

	// ���׸��� �ٲ��ֱ�
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