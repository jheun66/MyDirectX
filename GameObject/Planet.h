#pragma once

class Planet :public Sphere
{
public:
	Planet(wstring texturePath, string tag);
	~Planet();

	void Update() override;
	void Render() override;
	void PostRender();

	void CreateUVNormal();

	void SetPivot(XMFLOAT3 *value);


	XMFLOAT3 *pivot;
	UINT distance;
	float revSpeed = 20.0f;
	float degree = 0;
private:
	vector<VertexUVNormal> UVVertices;

	Texture* texture;


	UINT sector;
	UINT stack;

	string tag;
};