#pragma once

typedef VertexUVNormalTangent ModelVertex;

// ���ϴ°͸� ���� �̾Ƽ� ������
struct MaterialData
{
	string name;

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;

	string diffuseFile;
	string specularFile;
	string normalFile;
};

struct MeshData
{
	string name;
	string materialName;

	vector<ModelVertex> vertices;
	vector<UINT> indices;
};