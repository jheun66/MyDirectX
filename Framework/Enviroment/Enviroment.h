#pragma once

inline void IASetPT(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	DC->IASetPrimitiveTopology(type);
}

class Enviroment
{
private:
	Enviroment();
	~Enviroment();

	void CreateViewort();
	void CreateSamplerState();

public:
	static Enviroment* Get() { return instance; }
	static void Create() { instance = new Enviroment(); }
	static void Delete() { delete instance; }


private:
	SamplerState* samplerState;

	static Enviroment* instance;
};