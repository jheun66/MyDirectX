#pragma once

class Quad : public Transform
{
private:
	typedef VertexUV VertexType;
	
	Mesh* mesh;
	Material* material;

	vector<VertexType> vertices;
	vector<UINT> indices;

	ID3D11ShaderResourceView* srv;
public:
	Quad();
	~Quad();

	void Update();
	void Render();

	void CreateData();

	void SetTexure(wstring file);
	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }
};