#pragma once

class Environment
{
private:
	SamplerState* samplerState;
	D3D11_VIEWPORT viewport;

	LightBuffer* lightBuffer;


	static Environment* instance;

public:
	static Environment* Get() { return instance; }
	static void Create() { instance = new Environment(); }
	static void Delete() { delete instance; }

	// ImGui로 정보 출력
	void PostRender();

	LightBuffer* GetLight() { return lightBuffer; }

	void SetViewport(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);

private:
	Environment();
	~Environment();

	void CreateViewort();
	void CreateSamplerState();

};