#include <iostream>
#include <conio.h>
#include <tchar.h>
#include <cmath>
#include <list>
#include <fstream>

using namespace std;

struct ItemInfo
{
	char name[20];
	int price;
};

int main(void)
{
	ItemInfo item;

	for(int i=0; i<10; ++i)
	{
		cout << "name : ";
		cin >> item.name;
		cout << "price : ";
		cin >> item.price;

		//write form file
	}

	//read form file

	fstream file;

	//���̳ʸ����� �޸�ũ��� ������ �ȴ�
	file.open("data.dat", ios_base::out | ios_base::binary); //���¸�� : �ؽ�Ʈ���(���̳ʸ���尡 �����Ǿ� ���� ������ �⺻�����), ���̳ʸ����

	int a = 11;

	file.write((char*)&a, sizeof(int));

	file.close();

	return 0;
}