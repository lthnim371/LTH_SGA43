#include <iostream>
#include <tchar.h>
#include <list>

using namespace std;

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
		test.pop_front(i);
	}

	return 0;
}