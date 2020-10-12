#pragma once

class ModelBone
{
public:
	int index;
	string name;

	int parentIndex;
	ModelBone* parent;

	XMMATRIX transform;
	vector<ModelBone*> children;
};