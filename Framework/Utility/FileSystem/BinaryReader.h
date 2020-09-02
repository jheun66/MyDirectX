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
	XMFLOAT4X4 Float4x4();

	void Byte(void** data, UINT dataSize);

private:
	HANDLE file;
	DWORD size;
};