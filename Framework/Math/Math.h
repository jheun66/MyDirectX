#pragma once

namespace GameMath
{
	float Saturate(const float& value);

	int Random(int min, int max);
	float Random(float min, float max);

	float Distance(const Vector3& v1, const Vector3& v2);

	float Lerp(float start, float end, float deltaTime);

	const XMVECTORF32 kRight = { 1, 0, 0 };
	const XMVECTORF32 kUp = { 0, 1, 0 };
	const XMVECTORF32 kForward = { 0, 0, 1 };
}