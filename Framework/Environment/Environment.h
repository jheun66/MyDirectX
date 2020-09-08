#pragma once

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

	DirLightBuffer* GetLight() { return lightBuffer; }

private:
	SamplerState* samplerState;

	DirLightBuffer* lightBuffer;


	static Environment* instance;
};