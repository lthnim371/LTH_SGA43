#include <iostream>
#include <tchar.h>
#include <list>

using namespace std;

#pragma push_macro("new")
void* operator (size_t size)
{
	void* ptr = malloc(size);

	cout << "allocate : " << ptr << endl;
	return ptr;
}
#pragma pop_macro("new")
#pragma push_macro("delete")
void operator delete(
#pragma push_macro("delete")

int main(void)
{
	list<int> test;

	for(int i=0; i<10; ++i)
	{
		test.push_back(i);
	}

	list<int>::iterator it; //iterator : 포인터와 비슷하기 때문에
	for(it = test.begin(); it != test.end(); it++)
	{
		cout << *it << endl; //가리키는 값으로 출력
	}

	for(int i=0; i<10; ++i)
	{
		//동적할당한 만큼 해제가 된다
		it = test.begin();
		delete *it;
		test.pop_front();
	}

	return 0;
}