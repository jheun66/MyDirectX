#pragma once

class ModelReader
{
public:
	ModelReader();
	~ModelReader();

	void ReadFile(string file);

	////////////////////////////////////////////////////////
	//Material//////////////////////////////////////////////
	void ExportMaterial(string savePath);

	void ReadMaterial();
	void WriteMaterial(string savePath);
	string WriteTexture(string savePath, string file);

	////////////////////////////////////////////////////////
	//Mesh//////////////////////////////////////////////////
	void ExportMesh(string savePath);

	void ReadMeshData(aiNode* node);
	void WriteMeshData(string savePath);

private:
	string file;

	Assimp::Importer* importer;
	const aiScene* scene;

	vector<MaterialData*> materials;
	vector<MeshData*> meshes;
};