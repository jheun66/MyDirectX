#pragma once

// 단순히 서있는 모델 렌더링
class ModelRender : public Model
{
private:
	XMMATRIX transforms[MAX_MODEL_BONE];

public:
	ModelRender(string file);
	~ModelRender();

	void Update();
	void Render();

	void UpdateTransform();
};