#pragma once

#include <window.h>
#include <tchar.h>
#include <map>

template<typename ReturnType, typename FunctionPointer>
class Callable
{
public :
	Callable(FunctionPointer a):func(a){}

	template<typename Class,
		typename Arg1,
		typename Arg2,
		typename Arg3,
		typename Arg4>
		ReturnType operator ()(Class* pObj, Arg1 a1, Arg2 a2)
	{
		return (pObj->*func)(a1, a2);
	}

	//���ø��� �ٲ���� �Ѵ�....???? => �׳� ���߿� �����ð�����

private :
	FunctionPointer func;
};

class MainWindow
{
	typedef Callable<LRESULT, LRESULT(MainWindow::*)(HWND, UINT, WPARAM, LPARAM);
	typedef std::map<UINT, Action> EventMap;
	typedef EventHandler

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
			this); //�ڱ��ڽ��� �ѱ��(���� ����)

		if (hMainWnd == NULL)
		{
			DWORD dwError = ::GetLastError();

			//ErrorMessageBox(dwError);
			return 1;
		}

		::ShowWindow(hMainWnd, SW_NORMAL);
		::UpdateWindow(hMainWnd)
	}
protected:
	void SetEventMap()
	{
		//�ش� �����Ϳ� ����� �޽����� ���� ȣ��
	}

	//�ڵ鷯���� �̺�Ʈ���� ã�´�
		//������
			//������ ����

	
	//�ش� �޽����� ���� �۾��� �ϴ� �ڵ�(������ �Լ�ȭ)


	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		MainWindow* pWin; //�����ͷ� ����� �Ѿ������
		//this�� ���ӻ� lparam�� �� ���� �Ѿ��
		if(uMsg == WM_CREATE)
		{
			LPCREATESTRUCT lpcs;
			//reinterpret_cast ��ȯ 4���� ���� �ٽ� ���
			lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);

			pWin = reinterpret_cast<MainWindow*>(lpcs->

			//������ ����! //������� �� �ּҸ� ���� ����(���� ����)�ι�° ���ڿ� ����° ���ڸ� ������ �ϰڴ�
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, pWin);//ũ����Ʈ�� �ķδ� �ι�° ���ڸ� ��� ���� �ִ´� //�������;��� ������ 64��Ʈ

			return pWin->EventHandler(hWnd,uMsg,wParam,lParam);
		}
		else
		{
			//�������� �������͸� �����´�
			::GetWindowLongPtr(hWnd, GWLP_USERDATA, pWin);

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

		//���� �ʴ����

			//

		//������ �׳�
	}
private:

	
}