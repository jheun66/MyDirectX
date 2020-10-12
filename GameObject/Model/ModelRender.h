#pragma once

// 단순히 서있는 모델 렌더링
class ModelRender : public Model
{
private:
	vector<Transform*> transforms;
	XMMATRIX worlds[MAX_MODEL_INSTANCE];

	VertexBuffer* instanceBuffer;

public:
	ModelRender(string file);
	~ModelRender();

	void Update();
	void Render();

	void UpdateTransforms();

	Transform* AddTransform();
};