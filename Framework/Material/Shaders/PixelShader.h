#pragma once

class PixelShader : public Shader
{
	friend class Shader;
public:
	PixelShader(wstring file, string psName = "PS");
	~PixelShader();

	void Set();

private:
	ID3D11PixelShader* pixelShader = nullptr;
	ID3DBlob* pixelBlob = nullptr;
};