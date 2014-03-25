#include <windows.h>
#include <tchar.h> // for unicode

LRESULT CALLBACK MyWndProc(HWND,UINT,WPARAM,LPARAM); //�Լ�����...

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

	HWND hMainWnd = ::CreateWindowEx(0, lpClassName, //��ϵ� Ŭ���� �̸��̾�� ��
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

	::ShowWindow(hMainWnd, nCmdShow); //��ϵ� Ŭ�����̸� ����
	::UpdateWindow(hMainWnd); //�ֵ� ����

	MSG msg;

	while (true) //���ѷ����ε�....
	{
		if (::GetMessage(&msg, 0, 0, 0)) //�޽���queue���� �����´�
		{
			::TranslateMessage(&msg); //��� 
			::DispatchMessage(&msg); //�޽����� ó���ϴ� �۾�
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
