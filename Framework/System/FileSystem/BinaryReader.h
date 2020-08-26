#pragma once

class BinaryReader
{
public:
	BinaryReader(wstring filePath);
	~BinaryReader();

	int Int();
	UINT UInt();
	float Float();
	string String();

	void Byte(void** data, UINT dataSize);

private:
	HANDLE file;
	DWORD size;
};