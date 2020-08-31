#include "framework.h"

WarChief::WarChief()
	:Model("WarChief/WarChief")
{
	SetShader(L"Model");
	settingBuffer = new MSBuffer();
}

WarChief::~WarChief()
{
	delete settingBuffer;
}

void WarChief::Render()
{
	settingBuffer->SetBufferToPS(10);
	__super::Render();
}
