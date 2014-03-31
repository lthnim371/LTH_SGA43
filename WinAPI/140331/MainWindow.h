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
	//템플릿을 바꿔줘야 한다....???? => 그냥 나중에 수업시간에ㅋ
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
		wcex.lpfnWndProc = &MainWindow::MyWndProc; //static이라서
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
			this); //자기자신을 넘긴다(원형 참고)

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
		//해당 데이터에 저장된 메시지에 따른 호출
		AddEventHandler(WM_DESTROY, &Me::OnDestroy);
	}
	// 새로운 이벤트 처리기 등록.
	void AddEventHandler(UINT msg, Action act)
	{
		Handler it = eventmap.find(msg); //핸들러에서 이벤트맵을 찾는다
		if (it == eventmap.end()) //없으면
		{
			eventmap.insert(std::make_pair(msg, act)); //데이터 저장
		}
	}
	
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::PostQuitMessage(0);
		return 0;
	}
	
	//해당 메시지에 대한 작업을 하는 코드(일종의 함수화) -> ClockApp


	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		MainWindow* pWin; //포인터로 만들고 넘어왔을때
		//this의 쓰임새 lparam에 그 값이 넘어옴
		if(uMsg == WM_CREATE)
		{
			LPCREATESTRUCT lpcs;
			//reinterpret_cast 반환 4가지 설명 다시 듣기
			lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);

			pWin = reinterpret_cast<MainWindow*>(lpcs->lpCreateParams);

			//저장을 하자! //윈도우즈에 그 주소를 저장 가능(원형 참고)두번째 인자에 세번째 인자를 저장을 하겠다
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, LONG(pWin));//크레이트한 후로는 두번째 인자를 계속 갖고 있는다 //롱포인터쓰는 이유는 64비트

			return pWin->EventHandler(hWnd,uMsg,wParam,lParam);
		}
		else
		{
			//윈도우의 롱포인터를 가져온다
			pWin = reinterpret_cast<MainWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

			if(pWin) //제대로 가져왔으면
			{
				return pWin->EventHandler(hWnd,uMsg,wParam,lParam); //EventHandler에서 처리를 한다
			}
		}

		return ::DefWindowProc(hWnd,uMsg,wParam,lParam);

	}
	LRESULT EventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		//어떠한 메시지가 왔을때 그거에 대한 이벤트 찾기
		Handler it = eventmap.find(uMsg);

		if (it != eventmap.end()) //없지 않더라면
		{
			return (it->second)(this, hWnd, uMsg, wParam, lParam);
		}

		//없으면 그냥
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
protected :
	HINSTANCE hMainInst;
	HWND hMainWnd;

private :
	EventMapType eventmap;
};