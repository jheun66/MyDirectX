#pragma once

typedef VertexUVNormalTangent ModelVertex;

// 원하는것만 따로 뽑아서 쓰도록
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