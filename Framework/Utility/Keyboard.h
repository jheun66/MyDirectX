#pragma once

#define MAX_INPUT_KEY 255


class Keyboard
{
public:

	static Keyboard* Get() { return instance; }
	static void Create() { instance = new Keyboard(); }
	static void Delete() { delete instance; }

	void Update();


	bool Down(DWORD key) { return keyMap[key] == KEY_INPUT_STATUS_DOWN; }
	bool Up(DWORD key) { return keyMap[key] == KEY_INPUT_STATUS_UP; }
	bool Press(DWORD key) { return keyMap[key] == KEY_INPUT_STATUS_PRESS; }
	void ResetKeyState();

private:
	Keyboard();
	~Keyboard();


private:
	byte keyState[MAX_INPUT_KEY];
	byte keyOldState[MAX_INPUT_KEY];
	byte keyMap[MAX_INPUT_KEY];

	enum
	{
		KEY_INPUT_STATUS_NONE = 0,
		KEY_INPUT_STATUS_DOWN,
		KEY_INPUT_STATUS_UP,
		KEY_INPUT_STATUS_PRESS,
	};


	static Keyboard* instance;

};