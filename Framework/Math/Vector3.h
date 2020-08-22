#pragma once

class Vector3
{
public:
	// �⺻ ������
	Vector3();
	Vector3(XMFLOAT3 value);
	Vector3(float x, float y, float z);
	Vector3(XMVECTOR value);

	// ���� ������
	Vector3(const Vector3& src);
	// ���� ���� ������
	Vector3& operator=(const Vector3& rhs);

	// �̵� ������
	Vector3(Vector3&& src) noexcept;
	// �̵� ���� ������
	Vector3& operator=(Vector3&& rhs) noexcept;
	
	~Vector3() = default;

	// XMFLOAT3�� ��ȯ
	operator XMFLOAT3();

	// XMVECTOR�� ��ȯ
	operator XMVECTOR();

	// XMFLOAT3�� �� ����
	void SetX(float value);
	void SetY(float value);
	void SetZ(float value);
	float GetX();
	float GetY();
	float GetZ();

	// ����Ƽ�� ������Ƽó�� ���
	__declspec(property(get = GetX, put = SetX)) float x;
	__declspec(property(get = GetY, put = SetY)) float y;
	__declspec(property(get = GetZ, put = SetZ)) float z;

	// ���� �����
	Vector3 operator+(const Vector3& value) const;
	Vector3 operator-(const Vector3& value) const;

	void operator+=(const Vector3& value);
	void operator-=(const Vector3& value);

	Vector3 operator*(const float& value) const;
	Vector3 operator/(const float& value) const;

	void operator*=(const float& value);
	void operator/=(const float& value);


	// ���� �������δ� �������� �ʴ� ����, �ܼ��� ����� ���ؼ� ���� 
	Vector3 operator*(const Vector3& value) const;
	Vector3 operator/(const Vector3& value) const;
	void operator*=(const Vector3& value);
	void operator/=(const Vector3& value);
	Vector3 operator+(const float& value) const;
	Vector3 operator-(const float& value) const;
	void operator+=(const float& value);
	void operator-=(const float& value);

	// �ܺο��� �ش� �����ڸ� �̿��� ������ �� �� �ֵ���
	friend Vector3 operator+(const float value1, const Vector3& value2);
	friend Vector3 operator-(const float value1, const Vector3& value2);
	friend Vector3 operator*(const float value1, const Vector3& value2);
	friend Vector3 operator/(const float value1, const Vector3& value2);

	friend Vector3 operator+(const XMFLOAT3& value1, const Vector3& value2);
	friend Vector3 operator-(const XMFLOAT3& value1, const Vector3& value2);
	friend Vector3 operator*(const XMFLOAT3& value1, const Vector3& value2);
	friend Vector3 operator/(const XMFLOAT3& value1, const Vector3& value2);

	// ������ ���� ��ȯ
	float Length() const;		

	// ����ȭ�� ���͸� ��ȯ
	Vector3 Normal() const;		
	// ���� ���͸� ����ȭ ��Ŵ
	void Normalize();			

	// ���� ���Ϳ� ����
	Vector3 Cross(const Vector3& value) const;	
	// ���� ���Ϳ� ����
	float Dot(const Vector3& value) const;		

	// �� ���͸� ����
	static Vector3 Cross(const Vector3& vec1, const Vector3& vec2);
	// �� ���͸� ����
	static float Dot(const Vector3& vec1, const Vector3& vec2);

	static bool Approximation(float origin, float target, float approxValue);

	static bool Approximation(Vector3 origin, Vector3 target, float approxValue);

private:
	XMVECTOR data;
};