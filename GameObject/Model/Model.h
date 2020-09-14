#pragma once

class Model : public Transform
{
public:
	Model();
	Model(string file);
	~Model();

	virtual void Update();
	virtual void Render();

	void ReadMaterial(string file);
	void ReadMesh(string file);	

	void BindBone();
	void BindMesh();

	void SetShader(wstring file);
	void SetShader(wstring vsFile, wstring psFile);

	ModelBone* GetBoneByName(string name);

protected:
	ModelBone* root;

	map<string, Material*> materials;
	vector<ModelMesh*> meshes;
	vector<ModelBone*> bones;
	
};