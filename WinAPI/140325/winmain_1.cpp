#include <windows.h>
#include <windowsx.h> // GET_X_LPARAM, GET_Y_LPARAM
#include <tchar.h> // for unicode
#include <stdio.h>
#include <math.h>
#include <fstream>
#include "utility.h"

#pragma comment(lib, "winmm.lib")

LRESULT CALLBACK MyWndProc(HWND,UINT,WPARAM,LPARAM);

HINSTANCE hMainInst = NULL;

INT
APIENTRY
_tWinMain(HINSTANCE hInst,
		  HINSTANCE hPrevInst,
		  LPTSTR lpCmdLine,
		  INT nCmdShow)
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


	::ShowWindow(hMainWnd, nCmdShow);
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

LRESULT CALLBACK MyWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static SYSTEMTIME st;
	static float theta_h = 0.f;
	static float theta_m = 0.f;
	static float theta_s = 0.f;
	static POINT ptMouse = {-1,-1};
	static bool bSelectFont = false;
	static LOGFONT lfont;

	static int nCount = 5; //울리는 횟수를 위한 변수

	if (uMsg == WM_CREATE)
	{
		// WM_TIMER
		// typedef VOID (CALLBACK* TIMERPROC)(HWND, UINT, UINT_PTR, DWORD);

		std::fstream fontfile;

		fontfile.open("font.cfg", std::ios_base::binary | std::ios_base::in);

		if (fontfile.good())
		{
			fontfile.read((char*)&lfont, sizeof(lfont));
			bSelectFont = true;
		}

		fontfile.close();

		::SetTimer(hWnd,	// handle window
					0,			// timer id
					1000,		// millisecond time
					NULL);		// TIMEPROC

		::SetTimer(hWnd, 1, 2000, NULL);
	}
	else if (uMsg == WM_DESTROY)
	{
		::KillTimer(hWnd, 0);
		// SendMessage
		// PostMessage

		std::fstream fontfile;

		fontfile.open("font.cfg", std::ios_base::binary | std::ios_base::out);

		fontfile.write((char*)&lfont, sizeof(lfont));

		fontfile.close();


		::PostQuitMessage(0);
		return 0;
	}
	//else if (uMsg == WM_CLOSE)
	//{
	//	if (IDOK == ::MessageBox(NULL, _T("Really?"), _T("Confirm"), MB_OKCANCEL))
	//	{
	//		::DestroyWindow(hWnd);
	//	}
	//	return 0;
	//}
	else if (uMsg == WM_PAINT)
	{
		LPCTSTR lpszHello = _T("Welcome to Hell World");
		RECT rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		// 기본적으로 그리기는 WM_PAINT 처리시에 한다.

		::InflateRect(&rc, -50, -50);

		// 펜 만들고, DC 가 선택하도록 하자.
		HPEN GreenPen = ::CreatePen(PS_SOLID, 5, RGB(100,200,100));
		HPEN oldPen = (HPEN)::SelectObject(hdc, GreenPen);

		// 붓 만들고, DC 가 선택하도록...
		HBRUSH RedBrush = ::CreateSolidBrush(RGB(200,100,100));
		HBRUSH oldBrush = (HBRUSH)::SelectObject(hdc, RedBrush);
		
		// 무언가 그리고...
		//::FillRect(hdc, &rc, RedBrush);

		::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

		::Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);

		int cx = (rc.right + rc.left)/2;
		int cy = (rc.right + rc.left)/2;

		const int length = 100;
		POINT pt;

		float pi = 2.f*asin(1.f);
		float D2R = pi/180.f;

		int x = cx + length*sin(theta_h*D2R);
		int y = cy - length*cos(theta_h*D2R);

		::MoveToEx(hdc, cx, cy, &pt);
		::LineTo(hdc, x, y);

		x = cx + length*sin(theta_m*D2R);
		y = cy - length*cos(theta_m*D2R);

		::MoveToEx(hdc, cx, cy, &pt);
		::LineTo(hdc, x, y);

		x = cx + length*sin(theta_s*D2R);
		y = cy - length*cos(theta_s*D2R);

		::MoveToEx(hdc, cx, cy, &pt);
		::LineTo(hdc, x, y);


		HFONT oldFont;
		HFONT hFont;
		if (bSelectFont)
		{
			hFont = ::CreateFontIndirect(&lfont);

			oldFont = (HFONT)::SelectObject(hdc, hFont);
		}


		TCHAR lpszTime[10];

		_stprintf_s(lpszTime, _countof(lpszTime), _T("%02d:%02d:%02d"), 
			st.wHour, st.wMinute, st.wSecond);

		::DrawText(hdc, lpszTime, -1, &rc, DT_CENTER);

		TCHAR lpszMouse[20];
		_stprintf_s(lpszMouse, _countof(lpszMouse), _T("(%d,%d)"), 
			ptMouse.x, ptMouse.y);

		::DrawText(hdc, lpszMouse, -1, &rc, DT_LEFT | DT_TOP);

		if (bSelectFont)
		{
			::SelectObject(hdc, oldFont);
			::DeleteObject(hFont);
		}


		// 해제
		::SelectObject(hdc, oldBrush);
		::DeleteObject(RedBrush);

		::SelectObject(hdc, oldPen);
		::DeleteObject(GreenPen);

		::EndPaint(hWnd, &ps);
	}
	else if (uMsg == WM_TIMER)
	{
		if(wParam == 0)
		{
		::GetLocalTime(&st);
		//::GetSystemTime(&st);

		RECT rc;
		::GetClientRect(hWnd, &rc);

		// 12h : 360 = 1h : x, x = 360*1/12 = 30
		// 60m : 30 = 1m : x, x = 30*1/60 = 1/2
		// 60s : 1/2 = 1s : x, x = (1/2)/60 = 1/120
		theta_h = (st.wHour%12)*30 + (st.wMinute)/2 + (st.wSecond)/120;

		// 60m : 360 = 1m : x, x = 360*1/60 = 6
		// 60s : 6 = 1s : x, x = 6*1/60 = 1/10
		theta_m = (st.wMinute)*6 + (st.wSecond)/10;

		// 60s : 360 = 1s : x, x = 360*1/60 = 6
		theta_s = (st.wSecond)*6;

			if (st.wSecond == 0 || st.wSecond == 30) //30초마다
			//if (st.wSecond == 0) //매 분마다
			//if(st.wMinute = 0 && st.wSecond = 0) //매 시간마다
			{	
				::SetTimer(hWnd, 1, 2000, NULL);
				nCount = 5;
				//nCount = st.wHoure % 12; //시간에 따라 울리는 횟수
			}
			::InvalidateRect(hWnd, &rc, TRUE);
			}
			else if(wParam == 1)
			{
				nCount--; //한번 울릴때마다 감소
				::PlaySound(_T("cuckoo.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
				if(nCount < 0) //5번 다 울리면 
				{
					::KillTimer(hWnd, 1); //타이머 제거
				}
			}
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		// lParam : high-order : x, lower-order : y
		ptMouse.x = GET_X_LPARAM(lParam);
		ptMouse.y = GET_Y_LPARAM(lParam);

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);
	}
	else if (uMsg == WM_RBUTTONDOWN)
	{
		CHOOSEFONT cfont;
		cfont.lStructSize = sizeof(cfont);
		cfont.hwndOwner = hWnd;
		cfont.Flags = CF_EFFECTS | CF_SCREENFONTS | CF_FORCEFONTEXIST;
		cfont.lpLogFont = &lfont;

		if (::ChooseFont(&cfont))
		{
			bSelectFont = true;

			RECT rc;
			::GetClientRect(hWnd, &rc);
			::InvalidateRect(hWnd, &rc, TRUE);
		}
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{

		//HFONT hFont;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
