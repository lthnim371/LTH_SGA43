#include <iostream>
#include <map>

using namespace std;

int main(void)
{
	/*
	map<int, char*> test; //�տ��� key��, �ڿ��� data�� => Ű���� �����Ͱ��� ���������� ���� => ���ο��� pair��� ������ ������ ��(���� ����)

	test.insert(make_pair(0, "hello")); //<-�ϳ��� �����Ͷ�� ���� ��
	test.insert(make_pair(1, "world")); //�����ϴ� ����� insert�ۿ� ���� //���ϰ��� true, false�ε� iterator�� ���ÿ� ��ȯ�Ѵ� //Ű���� �ߺ����� �ʴ´ٸ� ����
	test.insert(make_pair(1, "good job")); // <= Ű���� �ߺ��Ǽ� ������ ������ �� �Ǿ���.
	*/

	map<const char*, const char*> test;

	test.insert(make_pair("hello", "hello"));
	test.insert(make_pair("world", "world"));
	test.insert(make_pair("mi", "good job"));
	

	//map<int, const char*>::iterator it;

	/*
	for(int i=0; i<10; i++)
	{
		it = test.find(i); //���� ���� ã�ƺ���
		if(it != test.end()) //���� �� ã���� end�� ��ȯ
			cout << it->first << " : " << it->second << endl;  //pair�� �۽�Ʈ�� ������� ������ �ִ�
		else
			cout << "����" << endl;
	}
	*/

	//cout << test[0] << endl; //test[0] : 0Ű���� ����Ű�� �༮�� ���Ѵ�


	//������� �ҷ���
	map<const char*, const char*>::iterator it;
	it = test.find("hello");
	it->second = "bye";

	cout << test["hello"] << endl;

	test["hello"] = "byebye"; //�̷����ϸ� ���� �����͸� �ٲ� �� �ִ�.

	cout << test["hello"] << endl;

	return 0;
}