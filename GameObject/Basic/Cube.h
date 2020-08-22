#pragma once

class Cube : public Transform
{
	typedef Vertex VertexType;
public:
	Cube();
	~Cube();

	void Update();
	void Render();

	void CreateData();

	void SetColor(XMFLOAT4 color) { colorBuffer->data.color = color; }

private:
	Mesh* mesh;
	Material* material;

	vector<VertexType> vertices;
	vector<UINT> indices;

	ColorBuffer* colorBuffer;
};