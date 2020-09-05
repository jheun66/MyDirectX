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

	void ReadBone(aiNode* node, int index, int parent);
	void ReadMesh(aiNode* node, int bone);
	void ReadSkin();
	void WriteMesh(string savePath);

	////////////////////////////////////////////////////////
	//Clip//////////////////////////////////////////////////
	void ExportClip(UINT index, string savePath);

	Clip* ReadClip(aiAnimation* animation);
	void ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& nodeInfos);
	void WriteClip(Clip* clip, string savePath);

private:
	// ±ª¿Ã?
	//string file;

	Assimp::Importer* importer;
	const aiScene* scene;

	vector<MaterialData*> materials;
	vector<MeshData*> meshes;
	vector<BoneData*> bones;
};