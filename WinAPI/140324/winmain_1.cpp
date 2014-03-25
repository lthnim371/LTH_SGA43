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

	//바이너리모드는 메모리크기로 저장이 된다
	file.open("data.dat", ios_base::out | ios_base::binary); //오픈모드 : 텍스트모드(바이너리모드가 설정되어 있지 않으면 기본모드임), 바이너리모드

	int a = 11;

	file.write((char*)&a, sizeof(int));

	file.close();

	return 0;
}