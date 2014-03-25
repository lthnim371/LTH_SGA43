#include <windows.h>
#include <tchar.h> // for unicode

LRESULT CALLBACK MyWndProc(HWND,UINT,WPARAM,LPARAM); //함수원형...

INT
WINAPI
_tWinMain(HINSTANCE hInst,
		  HINSTANCE hPrevInst,
		  LPTSTR lpCmdLine,
		  INT nCmdShow)
{
	LPCTSTR lpClassName = _T("MyMainWindow"); //

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
		return 2;
	}

	HWND hMainWnd = ::CreateWindowEx(0, lpClassName, //등록된 클래스 이름이어야 함
		_T("Hello World"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		hInst,
		0);

	if (hMainWnd == NULL)
	{
		DWORD dwError = ::GetLastError();
		return 1;
	}

	::ShowWindow(hMainWnd, nCmdShow); //등록된 클래스이면 실행
	::UpdateWindow(hMainWnd); //애도 실행

	MSG msg;

	while (true) //무한루프인데....
	{
		if (::GetMessage(&msg, 0, 0, 0)) //메시지queue에서 가져온다
		{
			::TranslateMessage(&msg); //어떠한 
			::DispatchMessage(&msg); //메시지를 처리하는 작업
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
	if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		LPCTSTR lpszHello = _T("Welcome to Hell World");
		RECT rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		::DrawText(hdc, lpszHello, -1, &rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);


		::EndPaint(hWnd, &ps);
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
