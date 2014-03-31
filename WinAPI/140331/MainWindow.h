#pragma once

#include <window.h>
#include <tchar.h>
#include <map>

template<typename ReturnType, typename Class, typename FunctionPointer>
class Callable
{
public :
	Callable(FunctionPointer a):func(a){}

	template<typename Derived,
		typename Arg1,
		typename Arg2,
		typename Arg3,
		typename Arg4>
	ReturnType operator ()(Derived* pObj, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		return (get_pointer(pObj)->*func)(a1, a2, a3, a4);
	}
	//���ø��� �ٲ���� �Ѵ�....???? => �׳� ���߿� �����ð�����
	template<typename Derived>
	Class* get_pointer(Derived* obj)
	{
		return dynamic_cast<Class*>(obj);
	}
private :
	FunctionPointer func;
};

template<typename T>
class MainWindow
{
	typedef MainWindow Me;
	typedef Callable<LRESULT, T, LRESULT(T::*)(HWND,UINT,WPARAM,LPARAM)> Action;
	typedef std::map<UINT, Action> EventMapType;
	typedef typename EventMapType::iterator Handler;
public:
	MainWindow()
	{
	}
	~MainWindow()
	{
	}
	BOOL Setup(HINSTANCE hInst)
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
		wcex.lpfnWndProc = &MainWindow::MyWndProc; //static�̶�
		wcex.lpszClassName = lpClassName;
		wcex.lpszMenuName = NULL;
		wcex.style = CS_VREDRAW | CS_HREDRAW;

		if (!::RegisterClassEx(&wcex))
		{
			DWORD dwError = ::GetLastError();

			//ErrorMessageBox(dwError);
			return FALSE;
		}

		int cx = ::GetSystemMetrics(SM_CXSCREEN);
		int cy = ::GetSystemMetrics(SM_CYSCREEN);

		cx = (cx-500)/2;
		cy = (cy-500)/2;

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
			this); //�ڱ��ڽ��� �ѱ��(���� ����)

		if (hMainWnd == NULL)
		{
			DWORD dwError = ::GetLastError();

			//ErrorMessageBox(dwError);
			return FALSE;
		}

		::ShowWindow(hMainWnd, SW_NORMAL);
		::UpdateWindow(hMainWnd)

		return TRUE;
	}
protected:
	virtual void SetEventMap()
	{
		//�ش� �����Ϳ� ����� �޽����� ���� ȣ��
		AddEventHandler(WM_DESTROY, &Me::OnDestroy);
	}
	// ���ο� �̺�Ʈ ó���� ���.
	void AddEventHandler(UINT msg, Action act)
	{
		Handler it = eventmap.find(msg); //�ڵ鷯���� �̺�Ʈ���� ã�´�
		if (it == eventmap.end()) //������
		{
			eventmap.insert(std::make_pair(msg, act)); //������ ����
		}
	}
	
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::PostQuitMessage(0);
		return 0;
	}
	
	//�ش� �޽����� ���� �۾��� �ϴ� �ڵ�(������ �Լ�ȭ) -> ClockApp


	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		MainWindow* pWin; //�����ͷ� ����� �Ѿ������
		//this�� ���ӻ� lparam�� �� ���� �Ѿ��
		if(uMsg == WM_CREATE)
		{
			LPCREATESTRUCT lpcs;
			//reinterpret_cast ��ȯ 4���� ���� �ٽ� ���
			lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);

			pWin = reinterpret_cast<MainWindow*>(lpcs->lpCreateParams);

			//������ ����! //������� �� �ּҸ� ���� ����(���� ����)�ι�° ���ڿ� ����° ���ڸ� ������ �ϰڴ�
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, LONG(pWin));//ũ����Ʈ�� �ķδ� �ι�° ���ڸ� ��� ���� �ִ´� //�������;��� ������ 64��Ʈ

			return pWin->EventHandler(hWnd,uMsg,wParam,lParam);
		}
		else
		{
			//�������� �������͸� �����´�
			pWin = reinterpret_cast<MainWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

			if(pWin) //����� ����������
			{
				return pWin->EventHandler(hWnd,uMsg,wParam,lParam); //EventHandler���� ó���� �Ѵ�
			}
		}

		return ::DefWindowProc(hWnd,uMsg,wParam,lParam);

	}
	LRESULT EventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		//��� �޽����� ������ �װſ� ���� �̺�Ʈ ã��
		Handler it = eventmap.find(uMsg);

		if (it != eventmap.end()) //���� �ʴ����
		{
			return (it->second)(this, hWnd, uMsg, wParam, lParam);
		}

		//������ �׳�
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
protected :
	HINSTANCE hMainInst;
	HWND hMainWnd;

private :
	EventMapType eventmap;
};