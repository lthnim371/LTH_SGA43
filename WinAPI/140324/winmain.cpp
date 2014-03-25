#include <windows.h>
#include <tchar.h> // for unicode
#include <math.h> //또는 <cmath>

#include <windowsx.h> //lParam가 들어 있다 //GET_X_LPARAM, GET_Y_LPARAM

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

	//타이틀바, 테두리 두께 구하는 함수
	int cx = ::GetSystemMetrics(SM_CXSCREEN); //모니터화면 크기를 반환
	int cy = ::GetSystemMetrics(SM_CYSCREEN); //모니터화면 크기를 반환
	//SM_C?FULLSCREEN : 작업표시줄을 제외한 사이즈

	//정가운데 화면을 구하기
	cx = (cx-500)/2;
	cy = (cy-500)/2;

	HWND hMainWnd = ::CreateWindowEx(0, lpClassName,
		_T("Hello World"),
		WS_OVERLAPPEDWINDOW,
		cx, //창의 위치(x)
		cy, //창의 위치(y)
		500, //그릴수 있는 영역(width)
		500, //그릴수 있는 영역(height) //타이틀qk,보더,테두리 다 합한 사이즈임
		0,
		0,
		hInst,
		0);

	if (hMainWnd == NULL)
	{
		DWORD dwError = ::GetLastError();
		return 1;
	}

	//WM_TIMER라는 시동어를 매번 실행한다.

	//SetTimer 함수 원형
	
	// typedef VOID (CALLBACK* TIMERPROC)(HWND, UINT, UINT_PTR, DWORD);
	/*
	::SetTimer(hMainWnd, //handle window
			0,			//timer id
			1000,		//millisecond time
			NULL);		//TIMEPROC //함수포인터를 넘겨야 한다. 안 넘기면 WM_TIMER라는 시동어를 주어진 시간마다 계속 발생을 시킨다.
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
	static LOGFONT lfont; //폰트이름이나 크기 등이 지정되어 있음 //페인트할때 포인트 생성해야 함

	
	//윈도우크레이트를 할때 발생하는 메시지 
	if(uMsg == WM_CREATE)
	{
		//크레이트와 동시에 셋타이머
		::SetTimer(hWnd,0,1000,NULL);

		std::fstream fontfile;

		fontfile.open("font.cfg", std::ios_base::binary | std::ios_base::in);

		if(fontfile.good()) //파일에 이상이 있으면 false를 반환함
		{
			fontfile.read((char*)&lfont, sizeof(lfont));
			bSelectFont = true;
		}

		fontfile.close();
	}
	

	if (uMsg == WM_DESTROY)
	{
		//만들었으면 꺼야한다
		::KillTimer(hWnd, NULL);

		::PostQuitMessage(0);

		//폰트로그 저장
		std::fstream fontfile;

		fontfile.open("font.cfg", std::ios_base::binary | std::ios_base::out);

		fontfile.write((char*)&lfont, sizeof(lfont));
		
		fontfile.close();

		return 0;
	}
	else if (uMsg == WM_PAINT) //InvalidateRect로 PAINT메시지가 쌓여서 초마다 호출하게 된다
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps); //HDC : 펜 같은 걸 가지고 있다.

		::InflateRect(&rc, -50, -50); //내부 창사이즈 조절 //이 함수로 사이즈를 조절하면 L = 50, R = R - 50 //정가운데 구할때 (R + L) /2 

		//기본적으로 그리기는 WM_PAINT 처리시에 한다.

		//펜 만들고, DC가 선택하도록 하자
		HPEN GreenPen = ::CreatePen(PS_SOLID, 5, RGB(100,200,100));
		HPEN oldPen = (HPEN)::SelectObject(hdc, GreenPen);

		//붓 만들고, DC가 선택하도록 하자
		HBRUSH RedBrush = ::CreateSolidBrush(RGB(200,100,100));
		HBRUSH oldBrush = (HBRUSH)::SelectObject(hdc, RedBrush); //c++이 아니라서 형변환을 해줘야 한다

		//무언가 그리는....
		//::FillRect(hdc, &rc, RedBrush); //상자를 브러쉬로 '채운다' ( , 사이즈 //테두리색이 없다

		::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom); //테두리 색이 있다(테두리펜을 사용한다)

		::Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);//상자를 정해놓고 가득 채우기


		//InflateRect함수로 사이즈를 조절하게 되면 L(T) = 50, R(B) = R(B) - 50이므로 
		int cx = (rc.right + rc.left) / 2;
		int cy = (rc.top + rc.bottom) / 2;

		const int length = 100;

		POINT pt;
		
		//float theta = 90.f; //이 숫자를 바꾸면 바늘 위치가 바뀜
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
		::MoveToEx(hdc, cx, cy, &pt); //선그리기 시작점
		::LineTo(hdc, cx + 50, cy + 50); //어디까지 그려라
		::MoveToEx(hdc, cx, cy, &pt); //선그리기 시작점
		::LineTo(hdc, cx - 50, cy + 50); //어디까지 그려라
		*/

		//붓
		::SelectObject(hdc, oldBrush);
		::DeleteObject(RedBrush);

		//펜
		::SelectObject(hdc, oldPen);
		::DeleteObject(GreenPen);

		HFONT hFont;
		HFONT oldFont;

		if(bSelectFont)
		{
			hFont = ::CreateFontIndirect(&lfont); //인다이렉트로 하면 로그포인터만 받음 //HFONT 반환
			oldFont = (HFONT)::SelectObject(hdc, hFont);;
		}


		TCHAR lpszTime[10];

		//시간 출력
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
	else if(uMsg == WM_TIMER) //매번 초마다 실행이 된다.....?
	{
		//시간 갖고오기
		::GetLocalTime(&st); //컴퓨터 시간을 가져옴
		//::GetSystemTime(&st); //국제 기준 시간을 가져옴

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
			//::PlaySound(_T("BreakTime"),hMainInst, SND_ASYNC | SND_RESOURCE); //웨이브 파일만 됨(기타 파일은 (웨이브로 압축한거니)웨이브를 풀거나 서드파티를 이용해야 함 //asinc : 비동기 (음성이 출력되면서 화면도 되기에)
		}

		//갖고 온 시간을 출력하기 위한 영역
		::InvalidateRect(hWnd, &rc, TRUE); //(인자, 영역, 지우고 그릴것인지 아닌지) //true : 지우고 다시 그려라 false: 지우지 말고 그려라??
	}
	//마우스로 좌표받아오기 WPARAM wParam,LPARAM lParam를 사용하는데 Lparam에 좌표값이 들어있다
	else if(uMsg == WM_MOUSEMOVE)
	{
		//lParam : high-order : x, lower-order : y
		//int x = HIWORD(lParam);
		//int y = LOWORD(lParam); //듀얼모니터에서 작동 안되서 사용하지 말라고 함
		ptMouse.x = GET_X_LPARAM(lParam); //듀얼모니터 작동됨
		ptMouse.y = GET_Y_LPARAM(lParam); //듀얼모니터 작동됨

		//바뀔때마다 그리라는 명령
		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);
	}
	else if(uMsg == WM_RBUTTONDOWN)
	{
		
		CHOOSEFONT cfont; //구조체 //인자값으로 필요하기에
		cfont.lStructSize = sizeof(cfont);
		cfont.hwndOwner = hWnd;
		cfont.Flags = CF_EFFECTS | CF_SCREENFONTS | CF_FORCEFONTEXIST;
		cfont.lpLogFont = &lfont; //저장됨 //내용 보충하시오 //파일 입출력 가능
		
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
		//폰트관리
		//HFONT hFont;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
