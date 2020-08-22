#include "Framework.h"

Vector3::Vector3()
{
	data = XMVectorSet(0, 0, 0, 0);
}

Vector3::Vector3(XMFLOAT3 value)
{
	data = XMLoadFloat3(&value);
}

Vector3::Vector3(float x, float y, float z)
{
	data = XMVectorSet(x, y, z, 0);
}

Vector3::Vector3(XMVECTOR value)
	: data(value)
{
}

Vector3::Vector3(const Vector3& src)
	:data(src.data)
{
}

Vector3& Vector3::operator=(const Vector3& rhs)
{
	// 자기 자신 대입하는지 체크
	if (this == &rhs)
	{
		return *this;
	}

	data = rhs.data;
	return *this;
}


Vector3::Vector3(Vector3&& src) noexcept
{
	data = src.data;
}

Vector3& Vector3::operator=(Vector3&& rhs) noexcept
{
	data = rhs.data;
	return *this;
}


Vector3::operator XMFLOAT3()
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, data);
	return result;
}

Vector3::operator XMVECTOR()
{
	return data;
}

void Vector3::SetX(float value)
{
	data = XMVectorSetX(data, value);
}

void Vector3::SetY(float value)
{
	data = XMVectorSetY(data, value);
}

void Vector3::SetZ(float value)
{
	data = XMVectorSetZ(data, value);
}

float Vector3::GetX()
{
	return XMVectorGetX(data);
}

float Vector3::GetY()
{
	return XMVectorGetY(data);
}

float Vector3::GetZ()
{
	return XMVectorGetZ(data);
}

Vector3 Vector3::operator+(const Vector3& value) const
{
	return Vector3(data + value.data);
}

Vector3 Vector3::operator-(const Vector3& value) const
{
	return Vector3(data - value.data);
}

void Vector3::operator+=(const Vector3& value)
{
	data += value.data;
}

void Vector3::operator-=(const Vector3& value)
{
	data -= value.data;
}

Vector3 Vector3::operator*(const float& value) const
{
	return data * XMVectorReplicate(value);
}

Vector3 Vector3::operator/(const float& value) const
{
	return data / XMVectorReplicate(value);
}

void Vector3::operator*=(const float& value)
{
	data *= XMVectorReplicate(value);
}

void Vector3::operator/=(const float& value)
{
	data /= XMVectorReplicate(value);
}

Vector3 Vector3::operator*(const Vector3& value) const
{
	return Vector3(data * value.data);
}

Vector3 Vector3::operator/(const Vector3& value) const
{
	return Vector3(data / value.data);
}

void Vector3::operator*=(const Vector3& value)
{
	data *= value.data;
}

void Vector3::operator/=(const Vector3& value)
{
	data /= value.data;
}

Vector3 Vector3::operator+(const float& value) const
{
	return data + XMVectorReplicate(value);
}

Vector3 Vector3::operator-(const float& value) const
{
	return data - XMVectorReplicate(value);
}

void Vector3::operator+=(const float& value)
{
	data += XMVectorReplicate(value);
}

void Vector3::operator-=(const float& value)
{
	data -= XMVectorReplicate(value);
}

float Vector3::Length() const
{
	// XMVector3Length(vector) : 반환되는 벡터의 x값에 길이를 들어있어있다.
	return XMVectorGetX(XMVector3Length(data));
}

Vector3 Vector3::Normal() const
{
	return XMVector3Normalize(data);
}

void Vector3::Normalize()
{
	data = XMVector3Normalize(data);
}

Vector3 Vector3::Cross(const Vector3& value) const
{
	return XMVector3Cross(data, value.data);
}

float Vector3::Dot(const Vector3& value) const
{
	return XMVectorGetX(XMVector3Dot(data, value.data));
}

Vector3 Vector3::Cross(const Vector3& vec1, const Vector3& vec2)
{
	return XMVector3Cross(vec1.data, vec2.data);
}
float Vector3::Dot(const Vector3& vec1, const Vector3& vec2)
{
	return XMVectorGetX(XMVector3Cross(vec1.data, vec2.data));
}

Vector3 operator+(const float value1, const Vector3& value2)
{
	return XMVectorReplicate(value1) + value2.data;
}

Vector3 operator-(const float value1, const Vector3& value2)
{
	return XMVectorReplicate(value1) - value2.data;
}

Vector3 operator*(const float value1, const Vector3& value2)
{
	return XMVectorReplicate(value1) * value2.data;
}

Vector3 operator/(const float value1, const Vector3& value2)
{
	return XMVectorReplicate(value1) / value2.data;
}

Vector3 operator+(const XMFLOAT3& value1, const Vector3& value2)
{
	return (Vector3)value1 + value2;
}

Vector3 operator-(const XMFLOAT3& value1, const Vector3& value2)
{
	return (Vector3)value1 - value2;
}

Vector3 operator*(const XMFLOAT3& value1, const Vector3& value2)
{
	return (Vector3)value1 * value2;
}

Vector3 operator/(const XMFLOAT3& value1, const Vector3& value2)
{
	return (Vector3)value1 / value2;
}

bool Vector3::Approximation(float origin, float target, float approxValue)
{
	return abs(origin - target) <= approxValue;
}

bool Vector3::Approximation(Vector3 origin, Vector3 target, float approxValue)
{
	bool result = true;

	result &= Approximation(origin.x, target.x, approxValue);
	result &= Approximation(origin.y, target.y, approxValue);
	result &= Approximation(origin.z, target.z, approxValue);

	return result;
}