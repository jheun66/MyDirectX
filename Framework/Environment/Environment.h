#pragma once

// ȯ�� ���� �������� ��� �ִ� Ŭ����(ī�޶�, ���� ��)
class Camera;

class Environment
{
private:
	Environment();
	~Environment();

	void CreateViewort();
	void CreateSamplerState();

public:
	static Environment* Get() { return instance; }
	static void Create() { instance = new Environment(); }
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


	static Environment* instance;
};