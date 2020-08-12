#pragma once

class PixelShader
{
public:
	PixelShader(wstring file, string psName = "PS");
	~PixelShader();

	void Set();

private:
	ID3D11PixelShader* pixelShader = nullptr;
	ID3DBlob* pixelBlob = nullptr;
};