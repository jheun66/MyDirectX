#pragma once
#include "GameObject/LandScape/Billboard.h"

class BillboardScene : public Scene
{
private:
	Terrain* terrain;

	//vector<Billboard*> trees;
	Material* material;
	GeometryShader* geometryShader;

	VertexBuffer* vertexBuffer;
	UINT count;

	BlendState* blendState[2];
	DepthStencilState* depthState[2];

	Snow* snow;
public:
	BillboardScene();
	~BillboardScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};