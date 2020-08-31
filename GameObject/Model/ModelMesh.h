#pragma once

class ModelMesh
{
	// Model���� ModelMesh�� �������ִ� �������
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
