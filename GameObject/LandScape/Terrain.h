#pragma once

class Terrain : Transform
{
	typedef VertexUVNormalTangent VertexType;
public:
	Terrain();
	~Terrain();

	void Update();
	void Render();
	void PostRender();

	bool Picking(OUT Vector3* position);
	float GetAltitude(Vector3 position);

private:
	void CreateData();
	void CreateNormal();
	void CreateTangent();

private:

	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT width, height;

	Texture* heightMap;

	// ���콺 ��ŷ
	Vector3 pickingPos;
	float altitude;
};