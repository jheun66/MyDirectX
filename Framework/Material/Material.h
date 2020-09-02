#pragma once

class Material
{
public:
	// Model�� �ִ� ������ Shader�� �־���� �Ѵ�.
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
	// Model���� ����� Material ������
	Material();

	Material(wstring file);
	Material(wstring vsFile, wstring psFile);
	Material(VertexShader* vertexShader, PixelShader* pixelShader);
	~Material();

	void Set();

	// �����ڿ��� �ٷ� �ȳְ� �ܺ� �𵨿� shader ������ �� ���
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