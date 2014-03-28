#include <iostream>
#include <conio.h>
#include <list>
#include <windows.h>
#include <map>

using namespace std;

template<typename ReturnType, typename FunctionPointer>
class Callable
{
public :
	Callable(FunctionPointer a):func(a){}

	template<typename Class,
		typename Arg1, typename Arg2>
		ReturnType operator ()(Class* pObj, Arg1 a1, Arg2 a2)
	{
		return (pObj->*func)(a1, a2);
	}
private :
	FunctionPointer func;
};

class MainWindow
{
	typedef Callable<LRESULT, LRESULT(MainWindow::*)(HWND,UINT,WPARAM,LPARAM)>Action;
public :
	MainWindow()
		: hMainWnd(NULL)
	{}

	void Setup()
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

		SetEventMap();


		hMainWnd = ::CreateWindowEx(0, lpClassName,
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

	}

	void SetEventMap()
	{
		EventMap.insert(WM_CREATE, &MainWindow::OnCreate);
		EventMap.insert(WM_DESTROY, &MainWindow::OnDestroy);
	}

	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		cout << "OnCreate called." << endl;
		return 0;
	}
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		cout << "OnDestroy called." << endl;
		return 0;
	}

private :
	HWND hMainWnd;
	std::map<UINT,Action> EventMap;
};



int main(void)
{
	WNDCLASSEX wcex;

	Test some;

	typedef Callable<long, long(Test::*)(int,int)> Action;

	Action EventHandler = &Test::OnCreate;

	long ret = EventHandler(&some, 1, 2);

	_getch();
	return 0;
}
