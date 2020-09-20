#pragma once

class Rain : public Particle
{
private:
	class RainBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			XMFLOAT3 velocity;
			float distance;

			XMFLOAT4 color;

			XMFLOAT3 origin;
			float time;

			XMFLOAT3 size;
			float padding;
		}data;

		RainBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.velocity = XMFLOAT3(0, -1, 0);
			data.distance = 1000.0f;

			data.color = XMFLOAT4(1, 1, 1, 1);

			data.origin = XMFLOAT3(0, 0, 0);
			data.size = XMFLOAT3(0, 0, 0);

			data.time = 0.0f;
		}
	};

	RainBuffer* rainBuffer;
	VertexSize* vertices;

public:
	Rain();
	~Rain();

	// Particle을(를) 통해 상속됨
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

};