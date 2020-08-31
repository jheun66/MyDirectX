#include "framework.h"

Tree::Tree()
	:Model("Tree/Tree")
{
	SetShader(L"Tree");
}

Tree::~Tree()
{
}

void Tree::Render()
{
	__super::Render();
}
