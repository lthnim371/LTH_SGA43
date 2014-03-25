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

//ㅇㄴㄹㄴㅁㅇㄹㄶ함수화
void Invalidate(HWND hWnd, BOOL bErase = TRUE)
{
	RECT rc;
	::GetClientRect(hWnd, &rc);
	::InvalidateRect(hWnd, &rc, bErase);
}

struct Point : public POINT
{
	Point(const LONG& _x, const LONG& _y)
	{
		x = _x;
		y = _y;
	}
	Point operator + (const Point& o)
	{
		return Point(x + o.x, y + o.y);
	}
};

class Circle
{
public:
	Circle(const Point& c, const LONG& r)
		: center(c), radius(r)
	{
	}
	void Draw(HDC hdc)
	{
		::Ellipse(hdc, center.x - radius, center.y - radius, center.x + radius, center.y + radius);

		POINT pt;
		::MoveToEx(hdc, center.x, center.y, &pt);
		::LineTo(hdc, center.x, center.y - (2 * radius));
	}
	void SetCenter(const Point& c)
	{
		center = c;
	}
	Point getCenter() const
	{
		return center;
	}
	operator Point()
	{
		return center;
	}
	Circle operator + (const Point& o) //래퍼런스로 안한 이유는 자기 자신을 수정하지 않고 별도로 객체를 만들어 반환한 것
	{
		return Circle(Point(center.x + o.x, center.y + o.y), radius);
	}

private:
	Point center;
	LONG radius;
};

void drawCircle(HDC hdc)
{
	::Ellipse(hdc, 200, 200, 250, 250);
}

Circle sample(Point(200,200), 15);

LRESULT CALLBACK MyWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		sample.Draw(hdc);

		::EndPaint(hWnd, &ps);
	}
	else if(uMsg == WM_KEYDOWN) //키보드를 누를때 발생하는 메시지
	{
		//wParam is Virtual-Key code
		if(wParam == VK_UP) //이와 같이 모든 키보드의 키들은 정의되어 있다
		{
			/*
			Point c = sample.getCenter();
			c.y -= 5;
			sample.SetCenter(c);
			*/
			sample += Point(0, -5);//더 간편하게 작성 또는 sample = sample + Point(0, -5);

			//다시 그려야 되므로
			Invalidate(hWnd);
		}
		else if(wParam == VK_DOWN)
		{
			Point c = sample.getCenter();
			c.y += 5;
			sample.SetCenter(c);

			Invalidate(hWnd);
		}
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
