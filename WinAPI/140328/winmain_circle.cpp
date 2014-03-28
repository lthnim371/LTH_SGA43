#include <windows.h>
#include <windowsx.h> // GET_X_LPARAM, GET_Y_LPARAM
#include <tchar.h> // for unicode
#include <stdio.h>
#include <math.h>
#include <list>
#include "utility.h"
#include "circle.h"

#pragma comment(lib, "winmm.lib")

LRESULT CALLBACK MyWndProc(HWND,UINT,WPARAM,LPARAM);

HINSTANCE hMainInst = NULL;

INT
WINAPI
_tWinMain(HINSTANCE hInst,
		  HINSTANCE ,
		  LPTSTR ,
		  INT )
{
	LPCTSTR lpClassName = _T("MyMainWindow");

	hMainInst = hInst;
	WNDCLASSEX wcex;

	::ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = hInst;
	wcex.lpfnWndProc = &MyWndProc;
	wcex.lpszClassName = lpClassName;
	wcex.lpszMenuName = NULL;
	wcex.style = CS_VREDRAW | CS_HREDRAW;

	if (!::RegisterClassEx(&wcex))
	{
		DWORD dwError = ::GetLastError();

		ErrorMessageBox(dwError);
		return 2;
	}

	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);

	cx = (cx-500)/2;
	cy = (cy-500)/2;

	HWND hMainWnd = ::CreateWindowEx(0, lpClassName,
		_T("Hello World"),
		WS_OVERLAPPEDWINDOW,
		cx,
		cy,
		500,
		500,
		0,
		0,
		hInst,
		0);

	if (hMainWnd == NULL)
	{
		DWORD dwError = ::GetLastError();

		ErrorMessageBox(dwError);
		return 1;
	}


	::ShowWindow(hMainWnd, SW_NORMAL);
	::UpdateWindow(hMainWnd);

	MSG msg;

	while (true)
	{
		//if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		if (::GetMessage(&msg, 0, 0, 0))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			break;
		}
	}


	return msg.wParam;
}


Circle sample(Point(200,200), 50);
std::list<Bomb*> Depot;
typedef std::list<Bomb*>::iterator iter;
Bomb* missile = NULL;
const int maxMissile = 20;

void Invalidate(HWND hWnd, BOOL bErase = TRUE)
{
	RECT rc;
	::GetClientRect(hWnd, &rc);
	::InvalidateRect(hWnd, &rc, bErase);
}

LRESULT CALLBACK MyWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		::SetTimer(hWnd, 0, 100, NULL);
	}
	else if (uMsg == WM_DESTROY)
	{
		iter it;
		for (it = Depot.begin(); it != Depot.end(); it++)
		{
			delete *it;
		}

		::KillTimer(hWnd, 0);
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		sample.Draw(hdc);
		std::list<Bomb*>::iterator it;
		for (it = Depot.begin(); it != Depot.end(); it++)
		{
			(*it)->Draw(hdc);
		}
		//if (missile)
		//{
		//	missile->Draw(hdc);
		//}

		::EndPaint(hWnd, &ps);
	}
	else if (uMsg == WM_TIMER)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);

		std::list<Bomb*>::iterator it;
		for (it = Depot.begin(); it != Depot.end();)
		{
			(*it)->move();

			if (!::PtInRect(&rc, (*it)->getCenter()))
			{
				delete *it;
				it = Depot.erase(it);
			}
			else
			{
				it++;
			}
		}

			//if (!::PtInRect(&rc, missile->getCenter()))
			//{
			//	delete missile;
			//	missile = NULL;
			//}

		Invalidate(hWnd);
		//}

		// GetKeyState()
		// GetAsyncKeyState()
		// GetKeyboardState()
		// wParam is Virtual-Key code.

		if ((::GetAsyncKeyState(VK_UP) & 0x8000) == 0x8000)
		{
			sample.move();
			Invalidate(hWnd);
		}
		if ((::GetAsyncKeyState(VK_DOWN) & 0x8000) == 0x8000)
		{
			sample.move(false);
			Invalidate(hWnd);
		}
		if ((::GetAsyncKeyState(VK_LEFT) & 0x8000) == 0x8000)
		{
			sample.turn(-5);
			Invalidate(hWnd);
		}
		if ((::GetAsyncKeyState(VK_RIGHT) & 0x8000) == 0x8000)
		{
			sample.turn(5);
			Invalidate(hWnd);
		}
		if ((::GetAsyncKeyState('1') & 0x8000) == 0x8000)
		{
			sample.gear(-1);
			Invalidate(hWnd);
		}
		if ((::GetAsyncKeyState('2') & 0x8000) == 0x8000)
		{
			sample.gear(1);
			Invalidate(hWnd);
		}
		if ((::GetAsyncKeyState(VK_SPACE) & 0x8000) == 0x8000)
		{
			Bomb* pBomb = new Bomb(sample.getCenter(), 5, sample.getAngle());
			if (Depot.size() == maxMissile)
			{
				std::list<Bomb*>::iterator it = Depot.begin();
				delete *it;
				Depot.pop_front();
			}
			Depot.push_back(pBomb);
			//missile = new Bomb(sample.getCenter(), 5, sample.getAngle());
		}
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}
