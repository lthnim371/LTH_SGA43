//원 안의 선을 좌우로 움직이게 만들기

#include <windows.h>
#include <windowsx.h> // GET_X_LPARAM, GET_Y_LPARAM
#include <tchar.h> // for unicode
#include <stdio.h>
#include <math.h>

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

		return 2;
	}

	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);

	cx = (cx-500)/2;
	cy = (cy-500)/2;

	HWND hMainWnd = ::CreateWindowEx(0, lpClassName,
		_T("원 안의 선 움직이기"),
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

//그려지는 영역 호출을 함수화
void Invalidate(HWND hWnd, BOOL bErase = TRUE)
{
	RECT rc;
	::GetClientRect(hWnd, &rc);
	::InvalidateRect(hWnd, &rc, bErase);
}

struct tagPoint : public POINT //POINT를 상속받은 구조체를 만듬
{
	tagPoint(const LONG& _x, const LONG& _y) //생성자를 이용하여 초기화
	{
		x = _x;
		y = _y;
	}
	/*
	tagPoint operator + (const tagPoint& o) //좌표 간 +가 가능하도록 연산자 정의
	{
		return tagPoint(x + o.x, y + o.y);
	}
	*/
};

class cCircle
{
private:
	tagPoint center;
	tagPoint line; //선을 관리할 멤버변수를 하나 더 만듬
	LONG radius;

public:
	cCircle(const tagPoint& c, const LONG& r) //초기화에 필요한 인자를 받는 생성자로 설정
		: center(c), radius(r), line(c.x, c.y - (r * 2)) //처음에는 선이 위를 향하게 설정
	{
	}
	void Draw(HDC hdc)
	{
		//구한 center좌표를 이용하여 radius의 값에 따라 크기 설정
		::Ellipse(hdc, center.x - radius, center.y - radius, center.x + radius, center.y + radius);
		
		//시작점과 도착점의 좌표값 인자를 각각 따로 한다.
		POINT pt;
		::MoveToEx(hdc, center.x, center.y, &pt);
		::LineTo(hdc, line.x, line.y);
	}
	void SetCenter(const tagPoint& c) //좌표값을 바꿔주는 함수
	{
		center = c;
	}
	void SetLine(const tagPoint& l) //좌표값을 바꿔주는 함수
	{
		line = l;
	}
	tagPoint getCenter() const //현재 좌표값을 리턴해주는 함수
	{
		return center;
	}
	tagPoint getLine() const //현재 좌표값을 리턴해주는 함수
	{
		return line;
	}
	int getRadius() const //현재 radius을 리턴
	{
		return radius * 2;
	}
	operator tagPoint() //형변환한다고 볼수있다. 포인터를 사용하면 center값을 반환한다....?
	{
		return center;
	}

	cCircle operator + (const tagPoint& o) //반환값을 래퍼런스로 안한 이유는 자기 자신을 수정하지 않고 별도로 객체를 만들어 반환하기 위함
	{
		return cCircle(tagPoint(center.x + o.x, center.y + o.y), radius);
	}
	
};

cCircle sample(tagPoint(250, 250), 30);

LRESULT CALLBACK MyWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static float theta = 90.f; //처음에는 위를 향하게 할거라서 90도로..

	if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		//drawCircle(hdc);
		sample.Draw(hdc);
		

		::EndPaint(hWnd, &ps);
	}
	else if(uMsg == WM_KEYDOWN) //WM_KEYDOWN : 키보드의 키를 누를때 발생하는 메시지
	{
		//필요한 변수 선언
		float pi = 2.f * asin(1.f);
		tagPoint center = sample.getCenter();
		tagPoint line = sample.getLine();
		int length = sample.getRadius();

		//wParam is Virtual-Key code
		if(wParam == VK_UP) //VK_UP와 같이 모든 키보드의 키들이 재정의되어 있다.
		{
			
			//tagPoint c = sample.getCenter(); //현재 좌표값을 받아온다.
			center.y -= 5; //적절한 값으로 마이너스(위를 향할거라서)
			sample.SetCenter(center); //수정한 좌표값으로 새로 셋팅해준다.
			
			//마찬가지로 동일하게 수정
			line.y -= 5;
			sample.SetLine(line);
			
			//더 간편하게 작성(대신에 연산자오버로딩을 작성해야한다.)
			//sample += tagPoint(0, -5); 또는
			//sample = sample + tagPoint(0, -5);

			Invalidate(hWnd); //바뀐 좌표값으로 다시 그린다.
		}
		else if(wParam == VK_DOWN)
		{
			
			//tagPoint c = sample.getCenter();
			center.y += 5;
			sample.SetCenter(center);
			
			line.y += 5;
			sample.SetLine(line);

			//sample = sample + tagPoint(0, +5);

			Invalidate(hWnd);
		}
		else if(wParam == VK_LEFT)
		{
			if(theta < 180) //선이 왼쪽으로 다 돌기전까지
			{
				theta += 5.f; //임의의 각도로 증가시킴
				//theta에 따른 좌표를 구함
				line.x = center.x + length * cos(theta*(pi/180.f));
				line.y = center.y - length * sin(theta*(pi/180.f));
				
				sample.SetLine(line); //수정한 좌표를 셋업

				Invalidate(hWnd);
			}
		}
		else if(wParam == VK_RIGHT)
		{
			if(theta > 0)
			{
				theta -= 5.f;
				line.x = center.x + length * cos(theta*(pi/180.f));
				line.y = center.y - length * sin(theta*(pi/180.f));
				
				sample.SetLine(line);

				Invalidate(hWnd);
			}

			Invalidate(hWnd);
		}
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
