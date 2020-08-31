#pragma once

class ModelMesh
{
	// Model에서 ModelMesh를 생성해주는 방식으로
	friend class Model;
private:
	ModelMesh();
	~ModelMesh();

public:
	void CreateMesh();
	void Render();

private:
	string name;
	string materialName;

	Material* material;
	Mesh* mesh;

	ModelVertex* vertices;
	UINT* indices;

	UINT vertexCount, indexCount;
};
