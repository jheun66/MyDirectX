#include "framework.h"

Zombie::Zombie()
	:Model("Zombie/Zombie")
{
	SetShader(L"Model");
	settingBuffer = new MSBuffer();
}

Zombie::~Zombie()
{
	delete settingBuffer;
}

void Zombie::Render()
{
	settingBuffer->SetBufferToPS(10);
	__super::Render();
}
