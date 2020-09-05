#include "framework.h"

Tree::Tree()
	:Model("Tree/Tree")
{
	SetShader(L"Model");
}

Tree::~Tree()
{
}

void Tree::Render()
{
	__super::Render();
}
