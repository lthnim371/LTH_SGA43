#include <iostream>

#include <map> //

using namespace std;

/*
void Swap(int a, int b)
{
	int c = a;
	a = b;
	b = c;
}
void Swap(float a, float b)
{
	float c = a;
	a = b;
	b = c;
}

//로직이 같을시 일반화가 가능하다
//typedef Some type; //Some는 어떠한 타입이 될 수 있다
template<typename T>
void Swap(T a, T b)
{
	T c = a;
	a = b;
	b = c;
}
*/

/*
template<typename T>
class Test
{
public:
	void set(T a)
	{
		meber = a;
	}
	T get() const
	{
		return meber;
	}
private:
	T meber;
};
*/

//함수포인터 클래스 //호출이 가능하다
template<typename ReturnType, typename FunctionPointer>
class Callable
{
public:
	Callable(FunctionPointer a):func(a){}

	template<typename Class,
		typename Arg1, typename Arg2>
	ReturnType operator()(Class* pObj, Arg1 a1, Arg2 a2)
	{
		return (pObj->*func)(a1, a2);
	}
private:
	FunctionPointer func;
};

/*
class Test
{
public:
	long OnCreate(int a, int b)
	{
		cout << a << ',' << b << endl;
	}
};
*/

class MainWindow
{
public:
	MainWindow()
		//:  초기화
	{}

	void Setup()
	{
		//윈도우 만드는 코드 복붙
	}
};

//WNDPROC 원형 설명 다시 듣기

int main(void)
{
	//list<int> test; //리스트를 사용할때도 해당 타입으로 생성하였었다....

	Test some;
	typedef  Callable<long, long(Test::*)(int, int)> Action;

	Action EventHandler = &Test::

	return 0;
}