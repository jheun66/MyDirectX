#pragma once

class ComputeShader : public Shader
{
	friend class Shader;
private:
	ComputeShader(wstring file, string csName = "CS");
	~ComputeShader();

public:
	virtual void Set() override;

private:
	ID3D11ComputeShader* shader;
	ID3DBlob* computeBlob;

};