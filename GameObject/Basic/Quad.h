#pragma once

class Quad : public Transform
{
	typedef VertexUV VertexType;
public:
	Quad();
	~Quad();

	void Update();
	void Render();

	void CreateData();

private:
	Mesh* mesh;
	Material* material;

	vector<VertexType> vertices;
	vector<UINT> indices;
};