#pragma once

class Material
{
public:
	Material();
	~Material();

	void Add(wstring vertexShaderPath, wstring pixelShaderPath,
		wstring texturePath);

	void Set(UINT textureSlot);

private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	Texture* texture;
};