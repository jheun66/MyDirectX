#pragma once

class Vector3
{
public:
	// 기본 생성자
	Vector3();
	Vector3(XMFLOAT3 value);
	Vector3(float x, float y, float z);
	Vector3(XMVECTOR value);

	// 복사 생성자
	Vector3(const Vector3& src);
	// 복사 대입 연산자
	Vector3& operator=(const Vector3& rhs);

	// 이동 생성자
	Vector3(Vector3&& src) noexcept;
	// 이동 대입 연산자
	Vector3& operator=(Vector3&& rhs) noexcept;
	
	~Vector3() = default;

	// XMFLOAT3로 변환
	operator XMFLOAT3();

	// XMVECTOR로 변환
	operator XMVECTOR();

	// XMFLOAT3의 각 값들
	void SetX(float value);
	void SetY(float value);
	void SetZ(float value);
	float GetX();
	float GetY();
	float GetZ();

	// 유니티의 프로터티처럼 사용
	__declspec(property(get = GetX, put = SetX)) float x;
	__declspec(property(get = GetY, put = SetY)) float y;
	__declspec(property(get = GetZ, put = SetZ)) float z;

	// 벡터 연산들
	Vector3 operator+(const Vector3& value) const;
	Vector3 operator-(const Vector3& value) const;

	void operator+=(const Vector3& value);
	void operator-=(const Vector3& value);

	Vector3 operator*(const float& value) const;
	Vector3 operator/(const float& value) const;

	void operator*=(const float& value);
	void operator/=(const float& value);


	// 벡터 개념으로는 존재하지 않는 연산, 단순히 계산을 위해서 만듬 
	Vector3 operator*(const Vector3& value) const;
	Vector3 operator/(const Vector3& value) const;
	void operator*=(const Vector3& value);
	void operator/=(const Vector3& value);
	Vector3 operator+(const float& value) const;
	Vector3 operator-(const float& value) const;
	void operator+=(const float& value);
	void operator-=(const float& value);

	// 외부에서 해당 연산자를 이용한 연산을 할 수 있도록
	friend Vector3 operator+(const float value1, const Vector3& value2);
	friend Vector3 operator-(const float value1, const Vector3& value2);
	friend Vector3 operator*(const float value1, const Vector3& value2);
	friend Vector3 operator/(const float value1, const Vector3& value2);

	friend Vector3 operator+(const XMFLOAT3& value1, const Vector3& value2);
	friend Vector3 operator-(const XMFLOAT3& value1, const Vector3& value2);
	friend Vector3 operator*(const XMFLOAT3& value1, const Vector3& value2);
	friend Vector3 operator/(const XMFLOAT3& value1, const Vector3& value2);

	// 벡터의 길이 반환
	float Length() const;		

	// 정규화된 벡터를 반환
	Vector3 Normal() const;		
	// 현재 벡터를 정규화 시킴
	void Normalize();			

	// 현재 벡터와 외적
	Vector3 Cross(const Vector3& value) const;	
	// 현재 벡터와 내적
	float Dot(const Vector3& value) const;		

	// 두 벡터를 외적
	static Vector3 Cross(const Vector3& vec1, const Vector3& vec2);
	// 두 벡터를 내적
	static float Dot(const Vector3& vec1, const Vector3& vec2);

	static bool Approximation(float origin, float target, float approxValue);

	static bool Approximation(Vector3 origin, Vector3 target, float approxValue);

private:
	XMVECTOR data;
};