#pragma once

// 저장한 순서대로 읽어주어야한다.
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