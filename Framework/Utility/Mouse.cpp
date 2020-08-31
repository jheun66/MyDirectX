#include "Framework.h"

Mouse* Mouse::instance = nullptr;

Mouse::Mouse()
{
	position = XMVectorSet( 0,0,0,0 );

	wheelStatus = XMVectorSet(0, 0, 0, 0);
	wheelOldStatus = XMVectorSet(0, 0, 0, 0);
	wheelMoveValue = XMVectorSet(0, 0, 0, 0);

	ZeroMemory(buttonStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonOldStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonMap, sizeof(byte) * MAX_INPUT_MOUSE);

	ZeroMemory(startDblClk, sizeof(DWORD) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonCount, sizeof(int) * MAX_INPUT_MOUSE);

	// ���� Windows�� �����Ǿ� �ִ� ����Ŭ�� �ð� ���� ��ȯ
	timeDblClk = GetDoubleClickTime();
	// OS������ ���� �ð� ��ȯ (msec ����)
	startDblClk[0] = GetTickCount64();

	for (int i = 1; i < MAX_INPUT_MOUSE; i++)
		startDblClk[i] = startDblClk[0];

	DWORD tLine = 0;
	// �Ķ���͸� ���� �������� �ý��� ������ �����ϰų� ���� ���� ���� ������ �Լ�
	// �ٿ� ���� ������ ���� 
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &tLine, 0);
}

Mouse::~Mouse()
{
}

void Mouse::Update()
{
	memcpy(buttonOldStatus, buttonStatus, sizeof(buttonOldStatus));

	ZeroMemory(buttonStatus, sizeof(buttonStatus));
	ZeroMemory(buttonMap, sizeof(buttonMap));

	// ���콺 ����, ������, ��� ��ư�� ���ȴ��� �ȴ��ȴ��� Ȯ��
	buttonStatus[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;

	// ������ button�� �������� �ȴ������� ���¿� ���Ͽ� ��� �������� �ƴ��� Ȯ�� 
	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		int tOldStatus = buttonOldStatus[i];
		int tStatus = buttonStatus[i];

		if (tOldStatus == 0 && tStatus == 1)
			buttonMap[i] = BUTTON_INPUT_STATUS_DOWN;
		else if (tOldStatus == 1 && tStatus == 0)
			buttonMap[i] = BUTTON_INPUT_STATUS_UP;
		else if (tOldStatus == 1 && tStatus == 1)
			buttonMap[i] = BUTTON_INPUT_STATUS_PRESS;
		else
			buttonMap[i] = BUTTON_INPUT_STATUS_NONE;
	}

	// ���콺 ��ġ �̵�
	POINT point;
	GetCursorPos(&point);
	// ��ü ȭ�� ������ Ŀ���� ��ǥ�� �ش� Ŭ���̾�Ʈ �������� ��ǥ�� ��ȯ 
	ScreenToClient(handle, &point);


	wheelOldStatus = XMVectorSetX(wheelOldStatus, XMVectorGetX(wheelStatus));
	wheelOldStatus = XMVectorSetY(wheelOldStatus, XMVectorGetY(wheelStatus));

	wheelStatus = XMVectorSetX(wheelStatus, float(point.x));
	wheelStatus = XMVectorSetY(wheelStatus, float(point.y));

	wheelMoveValue = wheelStatus - wheelOldStatus;


	// ������ ���� �ð��� tButtonStatus�� �ӽ÷� ����
	DWORD tButtonStatus = GetTickCount64();
	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		// ��ư�� ������ ��
		if (buttonMap[i] == BUTTON_INPUT_STATUS_DOWN)
		{
			if (buttonCount[i] == 1)
			{
				// ���� Ŭ�� ���ݺ��� Ŭ ��� �ٽ� 0����
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk)
					buttonCount[i] = 0;
			}
			buttonCount[i]++;

			if (buttonCount[i] == 1)
				startDblClk[i] = tButtonStatus;
		}

		// ��ư�� ���� ��
		if (buttonMap[i] == BUTTON_INPUT_STATUS_UP)
		{
			if (buttonCount[i] == 1)
			{
				// ���� Ŭ�� ���ݺ��� Ŭ ��� �ٽ� 0����
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk)
					buttonCount[i] = 0;
			}
			else if (buttonCount[i] == 2)
			{
				// ���� Ŭ��
				if ((tButtonStatus - startDblClk[i]) <= timeDblClk)
					buttonMap[i] = BUTTON_INPUT_STATUS_DBLCLK;

				buttonCount[i] = 0;
			}
		}//if
	}//for(i)
}


// lparam : ���콺 ��ġ(��ũ�� ��ǥ��)
// wparam : ���� ���忡�� ���콺 ��ư, shift, ctrl Ű�� ���¸� ��Ÿ���� �÷��� ��, ���� ����� ���� �󸶳� ���ȴ���
LRESULT Mouse::InputProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// ������ ���� ����� (0, 0)���� �Ѵ�.
	if (message == WM_LBUTTONDOWN || message == WM_MOUSEMOVE)
	{
		position = XMVectorSetX(position, (float)LOWORD(lParam));
		position = XMVectorSetY(position, (float)HIWORD(lParam));
	}

	if (message == WM_MOUSEWHEEL)
	{
		short tWheelValue = (short)HIWORD(wParam);

		wheelOldStatus = XMVectorSetZ(wheelOldStatus, XMVectorGetZ(wheelStatus));
		wheelStatus += XMVectorSetZ(wheelStatus, XMVectorGetZ(wheelStatus) + (float)tWheelValue);
	}

	return TRUE;
}

