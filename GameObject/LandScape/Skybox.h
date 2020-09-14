#pragma once

class Skybox
{
public:
	Skybox();
	~Skybox();

	void Render();
private:
	Sphere* sphere;
	Texture* cubeMap;

	DepthStencilState* depthMode[2];
};

