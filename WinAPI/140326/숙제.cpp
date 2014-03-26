//�� ���� ���� �¿�� �����̰� �����

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
		_T("�� ���� �� �����̱�"),
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

//�׷����� ���� ȣ���� �Լ�ȭ
void Invalidate(HWND hWnd, BOOL bErase = TRUE)
{
	RECT rc;
	::GetClientRect(hWnd, &rc);
	::InvalidateRect(hWnd, &rc, bErase);
}

struct tagPoint : public POINT //POINT�� ��ӹ��� ����ü�� ����
{
	tagPoint(const LONG& _x, const LONG& _y) //�����ڸ� �̿��Ͽ� �ʱ�ȭ
	{
		x = _x;
		y = _y;
	}
	/*
	tagPoint operator + (const tagPoint& o) //��ǥ �� +�� �����ϵ��� ������ ����
	{
		return tagPoint(x + o.x, y + o.y);
	}
	*/
};

class cCircle
{
private:
	tagPoint center;
	tagPoint line; //���� ������ ��������� �ϳ� �� ����
	LONG radius;

public:
	cCircle(const tagPoint& c, const LONG& r) //�ʱ�ȭ�� �ʿ��� ���ڸ� �޴� �����ڷ� ����
		: center(c), radius(r), line(c.x, c.y - (r * 2)) //ó������ ���� ���� ���ϰ� ����
	{
	}
	void Draw(HDC hdc)
	{
		//���� center��ǥ�� �̿��Ͽ� radius�� ���� ���� ũ�� ����
		::Ellipse(hdc, center.x - radius, center.y - radius, center.x + radius, center.y + radius);
		
		//�������� �������� ��ǥ�� ���ڸ� ���� ���� �Ѵ�.
		POINT pt;
		::MoveToEx(hdc, center.x, center.y, &pt);
		::LineTo(hdc, line.x, line.y);
	}
	void SetCenter(const tagPoint& c) //��ǥ���� �ٲ��ִ� �Լ�
	{
		center = c;
	}
	void SetLine(const tagPoint& l) //��ǥ���� �ٲ��ִ� �Լ�
	{
		line = l;
	}
	tagPoint getCenter() const //���� ��ǥ���� �������ִ� �Լ�
	{
		return center;
	}
	tagPoint getLine() const //���� ��ǥ���� �������ִ� �Լ�
	{
		return line;
	}
	int getRadius() const //���� radius�� ����
	{
		return radius * 2;
	}
	operator tagPoint() //����ȯ�Ѵٰ� �����ִ�. �����͸� ����ϸ� center���� ��ȯ�Ѵ�....?
	{
		return center;
	}

	cCircle operator + (const tagPoint& o) //��ȯ���� ���۷����� ���� ������ �ڱ� �ڽ��� �������� �ʰ� ������ ��ü�� ����� ��ȯ�ϱ� ����
	{
		return cCircle(tagPoint(center.x + o.x, center.y + o.y), radius);
	}
	
};

cCircle sample(tagPoint(250, 250), 30);

LRESULT CALLBACK MyWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static float theta = 90.f; //ó������ ���� ���ϰ� �ҰŶ� 90����..

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
	else if(uMsg == WM_KEYDOWN) //WM_KEYDOWN : Ű������ Ű�� ������ �߻��ϴ� �޽���
	{
		//�ʿ��� ���� ����
		float pi = 2.f * asin(1.f);
		tagPoint center = sample.getCenter();
		tagPoint line = sample.getLine();
		int length = sample.getRadius();

		//wParam is Virtual-Key code
		if(wParam == VK_UP) //VK_UP�� ���� ��� Ű������ Ű���� �����ǵǾ� �ִ�.
		{
			
			//tagPoint c = sample.getCenter(); //���� ��ǥ���� �޾ƿ´�.
			center.y -= 5; //������ ������ ���̳ʽ�(���� ���ҰŶ�)
			sample.SetCenter(center); //������ ��ǥ������ ���� �������ش�.
			
			//���������� �����ϰ� ����
			line.y -= 5;
			sample.SetLine(line);
			
			//�� �����ϰ� �ۼ�(��ſ� �����ڿ����ε��� �ۼ��ؾ��Ѵ�.)
			//sample += tagPoint(0, -5); �Ǵ�
			//sample = sample + tagPoint(0, -5);

			Invalidate(hWnd); //�ٲ� ��ǥ������ �ٽ� �׸���.
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
			if(theta < 180) //���� �������� �� ����������
			{
				theta += 5.f; //������ ������ ������Ŵ
				//theta�� ���� ��ǥ�� ����
				line.x = center.x + length * cos(theta*(pi/180.f));
				line.y = center.y - length * sin(theta*(pi/180.f));
				
				sample.SetLine(line); //������ ��ǥ�� �¾�

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
