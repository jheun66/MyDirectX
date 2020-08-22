#pragma once

class Material
{
public:
	Material(wstring file);
	Material(wstring vsFile, wstring psFile);
	Material(VertexShader* vertexShader, PixelShader* pixelShader);
	~Material();

	void Set();

	void SetDiffuseMap(wstring file) { diffuseMap = Texture::Add(file); }
	void SetSpecularMap(wstring file) { specularMap = Texture::Add(file); }
	void SetNormalMap(wstring file) { normalMap = Texture::Add(file); }

private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	Texture* diffuseMap;
	Texture* specularMap;
	Texture* normalMap;
};