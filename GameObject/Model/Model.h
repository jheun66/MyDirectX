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

	void SetShader(wstring file);

private:
	map<string, Material*> materials;
	vector<ModelMesh*> meshes;
};