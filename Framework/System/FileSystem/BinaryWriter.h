#pragma once

// ������ ������� �о��־���Ѵ�.
class BinaryWriter
{
public:
	BinaryWriter(wstring filePath);
	~BinaryWriter();

	void Int(int data);
	void UInt(UINT data);
	void Float(float data);
	void String(string data);

	void Byte(void* data, UINT dataSize);

private:
	HANDLE file;
	DWORD size;
};