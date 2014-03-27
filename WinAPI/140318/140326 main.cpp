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

	list<int>::iterator it; //iterator : �����Ϳ� ����ϱ� ������
	for(it = test.begin(); it != test.end(); it++)
	{
		cout << *it << endl; //����Ű�� ������ ���
	}

	for(int i=0; i<10; ++i)
	{
		//�����Ҵ��� ��ŭ ������ �ȴ�
		it = test.begin();
		delete *it;
		test.pop_front();
	}

	return 0;
}