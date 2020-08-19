#pragma once

class VertexShader;
class PixelShader;


class Shader
{
public:
	static VertexShader* AddVS(wstring file, string vsName = "VS");
	static PixelShader* AddPS(wstring file, string psName = "PS");
	static void Delete();

	virtual void Set() = 0;

private:
	static map<wstring, Shader*> totalShader;
};