#pragma once

// ȯ�� ���� �������� ��� �ִ� Ŭ����(ī�޶�, ���� ��)
class Camera;

class Enviroment
{
private:
	Enviroment();
	~Enviroment();

	void CreateViewort();
	void CreateSamplerState();

public:
	static Enviroment* Get() { return instance; }
	static void Create() { instance = new Enviroment(); }
	static void Delete() { delete instance; }

	// ImGui�� ���� ���
	void PostRender();

	Camera* MainCamera() { return mainCamera; }
	DirLightBuffer* GetLight() { return lightBuffer; }

private:
	SamplerState* samplerState;

	// ī�޶� �� ���� �ȵǸ� ���� �����ϱ�
	Camera* mainCamera;
	DirLightBuffer* lightBuffer;


	static Enviroment* instance;
};