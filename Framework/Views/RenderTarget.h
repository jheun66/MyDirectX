#pragma once

class RenderTarget
{
private:
	UINT width, height;

	ID3D11ShaderResourceView* srv;
	ID3D11Texture2D* rtvTexture;	// cpu에서 가지고 있을거?
	ID3D11RenderTargetView* rtv;

public:
	RenderTarget(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
	~RenderTarget();

	void Set(DepthStencil* depthStencil);

	static void Sets(RenderTarget** targets, UINT count, DepthStencil* depthStencil);

	// &안붙이면 바로 못 사용하고 임시변수 만들어줘야함
	ID3D11ShaderResourceView*& GetSRV() { return srv; }
	ID3D11RenderTargetView*& GetRTV() { return rtv; }

};