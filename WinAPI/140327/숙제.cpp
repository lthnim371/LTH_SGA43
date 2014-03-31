#include <windows.h>
#include <windowsx.h> // GET_X_LPARAM, GET_Y_LPARAM
#include <tchar.h> // for unicode
#include <stdio.h>
#include <math.h>
#include "utility.h"
#include "circle.h"
//#include "bomb.h"
#include <list>

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


Circle sample(Point(200,200), 20);
std::list<Bomb*> Depot;
typedef std::list<Bomb*>::iterator iter; //iterator을 typedef로 하여 쉽게 선언할 수 있도록 설정하였음
Bomb* missile = NULL;
const int maxMissile = 20; //미사일 제한 횟수

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
		for(it = Depot.begin(); it != Depot.end(); it++)
		{
			delete *it; //객체 메모리는 수동으로 해제해줘야 된다

			//따로 pop을 안한 이유는 list도 일종의 클래스라 소멸자에서 알아서 지워준다.
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

		/*
		if(missile)
		{
			missile->Draw(hdc);
		}
		*/
		std::list<Bomb*>::iterator it;
		for(it = Depot.begin(); it != Depot.end(); it++)
		{
			(*it)->Draw(hdc);
		}
		
		::EndPaint(hWnd, &ps);
	}
	else if (uMsg == WM_TIMER) //WM_KEYDOWN : 키값은 하나만 들어오기때문에 하나밖에 실행됨
	{
		//if(missile)
		//{
			
			//missile->move();

			RECT rc;
			::GetClientRect(hWnd, &rc);

			std::list<Bomb*>::iterator it;
			for(it = Depot.begin(); it != Depot.end(); )
			{
				(*it)->move();

				if(!::PtInRect(&rc, (*it)->getCenter()))
				{
					delete *it;
					it = Depot.erase(it); //인자값을 이터레이터로만 받음 //지우고 다음 녀석을 반환함
				}
				else
				{
					it++;
				}
			}

			/*
			

			if(::PtInRect(&rc, missile->getCenter())) //어떤 점이 어떠한 네모안에 있냐 없냐 //첫번째인자는 전체화면 크기, 두번째 인자는 센터로 설정하면 될듯
			{
				delete missile;
				missile = NULL;
			}
			*/

			Invalidate(hWnd);
		//}
		//키보드 받는 함수 /메시지와 상관없이 키보드 값을 받음
		// GetKeyState() //키보드 키 다 받음 // 배열로 넘겨줘야 함
		// GetAsyncKeyState() //키를 눌렀니 라고 물어볼때 씀 //게임쪽에서 많이 사용
		// GetKeyboardState()
		
		//타이머를 사용하여 매번 확인할 예정
		if ((::GetAsyncKeyState(VK_UP) & 0x8000) == 0x8000) //인자 : 버튼 키 값
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
		if ((::GetAsyncKeyState('1') & 0x8000) == 0x8000) //숫자와 알파뱃은 그냥 해당 ''으로 사용 가능
		{
			sample.gear(-1);
			Invalidate(hWnd);
		}
		if ((::GetAsyncKeyState('2') & 0x8000) == 0x8000)
		{
			sample.gear(1);
			Invalidate(hWnd);
		}
		if((::GetAsyncKeyState(VK_SPACE) & 0x8000) == 0x8000)
		{
			//missile = new Bomb(sample.getCenter(), 5, sample.getAngle());

			Bomb* pBomb = new Bomb(sample.getCenter(), 5, sample.getAngle());
			if(Depot.size() == maxMissile)
			{
				//그냥 pop만 하면 좀비 메모리가 생기므로
				std::list<Bomb*>::iterator it = Depot.begin();
				//delete *Depot.begin();
				delete *it;
				Depot.pop_front(); 
			}
			Depot.push_back(pBomb);
		}
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}
