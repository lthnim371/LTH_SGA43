#include <windows.h>
#include <tchar.h> // for unicode
#include <math.h> //�Ǵ� <cmath>

#include <windowsx.h> //lParam�� ��� �ִ� //GET_X_LPARAM, GET_Y_LPARAM

#include <fstream>

#include "utility.h"
//#pregma comment(lib, "

LRESULT CALLBACK MyWndProc(HWND,UINT,WPARAM,LPARAM);

HINSTANCE hMainInst = NULL;

INT
WINAPI
_tWinMain(HINSTANCE hInst,
		  HINSTANCE hPrevInst,
		  LPTSTR lpCmdLine,
		  INT nCmdShow)
{
	LPCTSTR lpClassName = _T("MyMainWindow");

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

	//Ÿ��Ʋ��, �׵θ� �β� ���ϴ� �Լ�
	int cx = ::GetSystemMetrics(SM_CXSCREEN); //�����ȭ�� ũ�⸦ ��ȯ
	int cy = ::GetSystemMetrics(SM_CYSCREEN); //�����ȭ�� ũ�⸦ ��ȯ
	//SM_C?FULLSCREEN : �۾�ǥ������ ������ ������

	//����� ȭ���� ���ϱ�
	cx = (cx-500)/2;
	cy = (cy-500)/2;

	HWND hMainWnd = ::CreateWindowEx(0, lpClassName,
		_T("Hello World"),
		WS_OVERLAPPEDWINDOW,
		cx, //â�� ��ġ(x)
		cy, //â�� ��ġ(y)
		500, //�׸��� �ִ� ����(width)
		500, //�׸��� �ִ� ����(height) //Ÿ��Ʋqk,����,�׵θ� �� ���� ��������
		0,
		0,
		hInst,
		0);

	if (hMainWnd == NULL)
	{
		DWORD dwError = ::GetLastError();
		return 1;
	}

	//WM_TIMER��� �õ�� �Ź� �����Ѵ�.

	//SetTimer �Լ� ����
	
	// typedef VOID (CALLBACK* TIMERPROC)(HWND, UINT, UINT_PTR, DWORD);
	/*
	::SetTimer(hMainWnd, //handle window
			0,			//timer id
			1000,		//millisecond time
			NULL);		//TIMEPROC //�Լ������͸� �Ѱܾ� �Ѵ�. �� �ѱ�� WM_TIMER��� �õ�� �־��� �ð����� ��� �߻��� ��Ų��.
	*/

	::ShowWindow(hMainWnd, nCmdShow);
	::UpdateWindow(hMainWnd);

	MSG msg;

	while (true)
	{
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

	static POINT ptMouse = {-1, -1};

	static bool bSelectFont = false;
	static LOGFONT lfont; //��Ʈ�̸��̳� ũ�� ���� �����Ǿ� ���� //����Ʈ�Ҷ� ����Ʈ �����ؾ� ��

	
	//������ũ����Ʈ�� �Ҷ� �߻��ϴ� �޽��� 
	if(uMsg == WM_CREATE)
	{
		//ũ����Ʈ�� ���ÿ� ��Ÿ�̸�
		::SetTimer(hWnd,0,1000,NULL);

		std::fstream fontfile;

		fontfile.open("font.cfg", std::ios_base::binary | std::ios_base::in);

		if(fontfile.good()) //���Ͽ� �̻��� ������ false�� ��ȯ��
		{
			fontfile.read((char*)&lfont, sizeof(lfont));
			bSelectFont = true;
		}

		fontfile.close();
	}
	

	if (uMsg == WM_DESTROY)
	{
		//��������� �����Ѵ�
		::KillTimer(hWnd, NULL);

		::PostQuitMessage(0);

		//��Ʈ�α� ����
		std::fstream fontfile;

		fontfile.open("font.cfg", std::ios_base::binary | std::ios_base::out);

		fontfile.write((char*)&lfont, sizeof(lfont));
		
		fontfile.close();

		return 0;
	}
	else if (uMsg == WM_PAINT) //InvalidateRect�� PAINT�޽����� �׿��� �ʸ��� ȣ���ϰ� �ȴ�
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps); //HDC : �� ���� �� ������ �ִ�.

		::InflateRect(&rc, -50, -50); //���� â������ ���� //�� �Լ��� ����� �����ϸ� L = 50, R = R - 50 //����� ���Ҷ� (R + L) /2 

		//�⺻������ �׸���� WM_PAINT ó���ÿ� �Ѵ�.

		//�� �����, DC�� �����ϵ��� ����
		HPEN GreenPen = ::CreatePen(PS_SOLID, 5, RGB(100,200,100));
		HPEN oldPen = (HPEN)::SelectObject(hdc, GreenPen);

		//�� �����, DC�� �����ϵ��� ����
		HBRUSH RedBrush = ::CreateSolidBrush(RGB(200,100,100));
		HBRUSH oldBrush = (HBRUSH)::SelectObject(hdc, RedBrush); //c++�� �ƴ϶� ����ȯ�� ����� �Ѵ�

		//���� �׸���....
		//::FillRect(hdc, &rc, RedBrush); //���ڸ� �귯���� 'ä���' ( , ������ //�׵θ����� ����

		::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom); //�׵θ� ���� �ִ�(�׵θ����� ����Ѵ�)

		::Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);//���ڸ� ���س��� ���� ä���


		//InflateRect�Լ��� ����� �����ϰ� �Ǹ� L(T) = 50, R(B) = R(B) - 50�̹Ƿ� 
		int cx = (rc.right + rc.left) / 2;
		int cy = (rc.top + rc.bottom) / 2;

		const int length = 100;

		POINT pt;
		
		//float theta = 90.f; //�� ���ڸ� �ٲٸ� �ٴ� ��ġ�� �ٲ�
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
		
		
		/*
		::MoveToEx(hdc, cx, cy, &pt); //���׸��� ������
		::LineTo(hdc, cx + 50, cy + 50); //������ �׷���
		::MoveToEx(hdc, cx, cy, &pt); //���׸��� ������
		::LineTo(hdc, cx - 50, cy + 50); //������ �׷���
		*/

		//��
		::SelectObject(hdc, oldBrush);
		::DeleteObject(RedBrush);

		//��
		::SelectObject(hdc, oldPen);
		::DeleteObject(GreenPen);

		HFONT hFont;
		HFONT oldFont;

		if(bSelectFont)
		{
			hFont = ::CreateFontIndirect(&lfont); //�δ��̷�Ʈ�� �ϸ� �α������͸� ���� //HFONT ��ȯ
			oldFont = (HFONT)::SelectObject(hdc, hFont);;
		}


		TCHAR lpszTime[10];

		//�ð� ���
		_stprintf_s(lpszTime, _countof(lpszTime), _T("%02d:%02d:%02d"), st.wHour, st.wMinute,st.wSecond);
		::DrawText(hdc, lpszTime, -1, &rc, DT_CENTER);

		TCHAR lpszMouse[20];
		_stprintf_s(lpszMouse, _countof(lpszMouse), _T("(%d,%d"), ptMouse.x, ptMouse.y);
		::DrawText(hdc, lpszMouse, -1, &rc, DT_LEFT | DT_TOP);

		::EndPaint(hWnd, &ps);

		if(bSelectFont)
		{
			::SelectObject(hdc, oldFont);
			::DeleteObject(hFont);
		}
	}
	else if(uMsg == WM_TIMER) //�Ź� �ʸ��� ������ �ȴ�.....?
	{
		//�ð� �������
		::GetLocalTime(&st); //��ǻ�� �ð��� ������
		//::GetSystemTime(&st); //���� ���� �ð��� ������

		RECT rc;
		::GetClientRect(hWnd, &rc);

		// 12h : 360 = 1h : x => x = 360*1/12 = 30
		// 60m : 30 = 1m : x => x = 30*1/60 = 1/2
		// 60s : 1/2 = 1s : x => x = (1/2)/60 = 1/120
		theta_h = (st.wHour%12)*30 + (st.wMinute)/2 + (st.wSecond)/120;

		// 60m : 30 = 1m : x => x = 360*1/60 = 6
		// 60s : 6 = 1s : x => x = 6*1/60 = 1/10
		theta_m = (st.wMinute)*6 + (st.wSecond)/10;

		// 60s : 360 = 1s : x => x = 6*1/60 = 1/10
		theta_s = (st.wSecond) * 6;

		if (st.wSecond == 0)
		{
			//::PlaySound(_T("BreakTime"),hMainInst, SND_ASYNC | SND_RESOURCE); //���̺� ���ϸ� ��(��Ÿ ������ (���̺�� �����ѰŴ�)���̺긦 Ǯ�ų� ������Ƽ�� �̿��ؾ� �� //asinc : �񵿱� (������ ��µǸ鼭 ȭ�鵵 �Ǳ⿡)
		}

		//���� �� �ð��� ����ϱ� ���� ����
		::InvalidateRect(hWnd, &rc, TRUE); //(����, ����, ����� �׸������� �ƴ���) //true : ����� �ٽ� �׷��� false: ������ ���� �׷���??
	}
	//���콺�� ��ǥ�޾ƿ��� WPARAM wParam,LPARAM lParam�� ����ϴµ� Lparam�� ��ǥ���� ����ִ�
	else if(uMsg == WM_MOUSEMOVE)
	{
		//lParam : high-order : x, lower-order : y
		//int x = HIWORD(lParam);
		//int y = LOWORD(lParam); //������Ϳ��� �۵� �ȵǼ� ������� ����� ��
		ptMouse.x = GET_X_LPARAM(lParam); //������� �۵���
		ptMouse.y = GET_Y_LPARAM(lParam); //������� �۵���

		//�ٲ𶧸��� �׸���� ���
		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);
	}
	else if(uMsg == WM_RBUTTONDOWN)
	{
		
		CHOOSEFONT cfont; //����ü //���ڰ����� �ʿ��ϱ⿡
		cfont.lStructSize = sizeof(cfont);
		cfont.hwndOwner = hWnd;
		cfont.Flags = CF_EFFECTS | CF_SCREENFONTS | CF_FORCEFONTEXIST;
		cfont.lpLogFont = &lfont; //����� //���� �����Ͻÿ� //���� ����� ����
		
		if(::ChooseFont(&cfont))
		{
			bSelectFont = true;

			RECT rc;
			::GetClientRect(hWnd, &rc);
			::InvalidateRect(hWnd, &rc, TRUE);
		}
	}
	else if(uMsg == WM_LBUTTONDOWN)
	{
		//��Ʈ����
		//HFONT hFont;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
