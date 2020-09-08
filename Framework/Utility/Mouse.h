#pragma once

#define MAX_INPUT_MOUSE 8

class Mouse
{
private:
	Mouse();
	~Mouse();

public:

	static Mouse* Get() { return instance; }
	static void Create() { instance = new Mouse(); }
	static void Delete() { delete instance; }


	void SetHandle(HWND handle)
	{
		this->handle = handle;
	}

	void Update();

	LRESULT InputProc(UINT message, WPARAM wParam, LPARAM lParam);

	XMVECTOR GetPosition() { return position; }

	bool Down(DWORD button)
	{
		return buttonMap[button] == BUTTON_INPUT_STATUS_DOWN;
	}

	bool Up(DWORD button)
	{
		return buttonMap[button] == BUTTON_INPUT_STATUS_UP;
	}

	bool Press(DWORD button)
	{
		return buttonMap[button] == BUTTON_INPUT_STATUS_PRESS;
	}

	XMVECTOR GetMoveValue()
	{
		return wheelMoveValue;
	}

private:

	static Mouse* instance;

	HWND handle;
	XMVECTOR position; //마우스 위치

	byte buttonStatus[MAX_INPUT_MOUSE];
	byte buttonOldStatus[MAX_INPUT_MOUSE];
	byte buttonMap[MAX_INPUT_MOUSE];

	Vector3 wheelStatus;
	Vector3 wheelOldStatus;
	Vector3 wheelMoveValue;

	DWORD timeDblClk;
	DWORD startDblClk[MAX_INPUT_MOUSE];
	int buttonCount[MAX_INPUT_MOUSE];

	enum
	{
		MOUSE_ROTATION_NONE = 0,
		MOUSE_ROTATION_LEFT,
		MOUSE_ROTATION_RIGHT
	};

	enum
	{
		BUTTON_INPUT_STATUS_NONE = 0,
		BUTTON_INPUT_STATUS_DOWN,
		BUTTON_INPUT_STATUS_UP,
		BUTTON_INPUT_STATUS_PRESS,
		BUTTON_INPUT_STATUS_DBLCLK
	};

	
};

