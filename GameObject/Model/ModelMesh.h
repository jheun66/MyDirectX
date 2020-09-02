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

	void Update();
	void Render();

	void SetTransforms(XMMATRIX* transforms);

	int BoneIndex() { return boneIndex; }

private:
	string name;

	int boneIndex;
	ModelBone* bone;

	vector<ModelMeshPart*> meshParts;
	Mesh* mesh;

	ModelVertex* vertices;
	UINT* indices;

	UINT vertexCount, indexCount;
};
