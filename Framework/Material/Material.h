#pragma once

class Material
{
public:
	// Model에 있는 정보들 Shader에 넣어줘야 한다.
	class MaterialBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			XMFLOAT4 diffuse;
			XMFLOAT4 specular;
			XMFLOAT4 ambient;
		}data;

		MaterialBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.diffuse = XMFLOAT4(1, 1, 1, 1);
			data.specular = XMFLOAT4(1, 1, 1, 1);
			data.ambient = XMFLOAT4(1, 1, 1, 1);
		}
	};

	string name;
public:
	// Model에서 사용할 Material 생성자
	Material();

	Material(wstring file);
	Material(wstring vsFile, wstring psFile);
	Material(VertexShader* vertexShader, PixelShader* pixelShader);
	~Material();

	void Set();

	// 생성자에서 바로 안넣고 외부 모델에 shader 적용할 때 사용
	void SetShader(wstring file);
	void SetShader(wstring vsFile, wstring psFile);

	void SetDiffuseMap(wstring file) { diffuseMap = Texture::Add(file); }
	void SetSpecularMap(wstring file) { specularMap = Texture::Add(file); }
	void SetNormalMap(wstring file) { normalMap = Texture::Add(file); }

	MaterialBuffer* GetBuffer() { return buffer; }
private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	Texture* diffuseMap;
	Texture* specularMap;
	Texture* normalMap;

	MaterialBuffer* buffer;
};