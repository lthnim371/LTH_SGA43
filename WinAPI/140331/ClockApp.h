#pragma once

#include "MainWindow.h"

class ClockApp : public MainWindow<ClockApp>
{
	typedef ClockApp Me;
protected :
	void SetEventMap()
	{
		//��� �޽����� ���ؼ�
		AddEventHandler(WM_CREATE, &Me::OnCreate);
		AddEventHandler(WM_PAINT, &Me::OnPaint);
		AddEventHandler(WM_TIMER, &Me::OnTimer);
	}

	//������ �۾��� �ҰŴ�
	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}
	LRESULT OnTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}
	LRESULT OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}
};
