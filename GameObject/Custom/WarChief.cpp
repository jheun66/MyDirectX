#include "framework.h"

WarChief::WarChief()
	:Model("WarChief/WarChief")
{
	SetShader(L"Model");
	settingBuffer = new SettingBuffer();
}

WarChief::~WarChief()
{
	delete settingBuffer;
}

void WarChief::Render()
{
	settingBuffer->SetPSBuffer(10);
	__super::Render();
}
