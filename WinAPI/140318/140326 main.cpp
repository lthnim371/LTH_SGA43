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

	list<int>::iterator it; //iterator : �����Ϳ� ����ϱ� ������
	for(it = test.begin(); it != test.end(); it++)
	{
		cout << *it << endl; //����Ű�� ������ ���
	}

	for(int i=0; i<10; ++i)
	{
		test.pop_front(i);
	}

	return 0;
}