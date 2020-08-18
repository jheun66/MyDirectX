#pragma once

class Star : public Sphere
{
public:
	Star();
	~Star();
	void Render() override;
	void PostRender() override;

	void SetColor(XMFLOAT4 color) { colorBuffer->data.color = color; }

private:
	ColorBuffer* colorBuffer;

};