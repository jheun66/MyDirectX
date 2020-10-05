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

	// ImGui로 정보 출력
	void PostRender();

	LightBuffer* GetLight() { return lightBuffer; }

private:
	SamplerState* samplerState;

	LightBuffer* lightBuffer;


	static Environment* instance;
};