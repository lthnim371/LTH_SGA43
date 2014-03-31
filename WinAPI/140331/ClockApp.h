#pragma once

#include "MainWindow.h"

class ClockApp : public MainWindow<ClockApp>
{
	typedef ClockApp Me;
protected :
	void SetEventMap()
	{
		//어떠한 메시지에 대해서
		AddEventHandler(WM_CREATE, &Me::OnCreate);
		AddEventHandler(WM_PAINT, &Me::OnPaint);
		AddEventHandler(WM_TIMER, &Me::OnTimer);
	}

	//각각의 작업을 할거다
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
