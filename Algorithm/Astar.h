#pragma once

class AStar
{
private:
	UINT width, height;

	vector<Node*> nodes;
	//vector<int> openNodes;
	Heap* heap;

	XMFLOAT2 interval;
	
	vector<BoxCollider*> obstacles;

public:
	AStar(UINT width = 20, UINT height = 20);
	~AStar();

	void Setup(class Terrain* terrain);

	void Update();
	void Render();

	int FindCloseNode(Vector3 pos);
	vector<Vector3> FindPath(int start, int end);
	void MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path);

	void Reset();

	bool isCollisionObstacle(Ray ray, float destDistance);

	void isObstacle(Collider* collider);

	// y가 일정 높이 이상이면 못가게함
	void isDungeon();

private:
	float GetDistance(int curIndex, int end);

	void Extend(int center, int end);
	int GetMinNode();
};