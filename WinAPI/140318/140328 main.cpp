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

//������ ������ �Ϲ�ȭ�� �����ϴ�
//typedef Some type; //Some�� ��� Ÿ���� �� �� �ִ�
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

//�Լ������� Ŭ���� //ȣ���� �����ϴ�
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
		//:  �ʱ�ȭ
	{}

	void Setup()
	{
		//������ ����� �ڵ� ����
	}
};

//WNDPROC ���� ���� �ٽ� ���

int main(void)
{
	//list<int> test; //����Ʈ�� ����Ҷ��� �ش� Ÿ������ �����Ͽ�����....

	Test some;
	typedef  Callable<long, long(Test::*)(int, int)> Action;

	Action EventHandler = &Test::

	return 0;
}