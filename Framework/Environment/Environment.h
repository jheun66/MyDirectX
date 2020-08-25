#pragma once

// 환경 설정 정보등을 담고 있는 클래스(카메라, 조명 등)
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

	// ImGui로 정보 출력
	void PostRender();

	Camera* MainCamera() { return mainCamera; }
	DirLightBuffer* GetLight() { return lightBuffer; }

private:
	SamplerState* samplerState;

	// 카메라 모를 쑤도 안되면 전역 선언하기
	Camera* mainCamera;
	DirLightBuffer* lightBuffer;


	static Environment* instance;
};