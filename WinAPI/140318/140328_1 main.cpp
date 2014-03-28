#include <iostream>
#include <map>

using namespace std;

int main(void)
{
	/*
	map<int, char*> test; //앞에는 key값, 뒤에는 data값 => 키값과 데이터값을 내부적으로 관리 => 내부에서 pair라는 단위로 저장을 함(원형 참고)

	test.insert(make_pair(0, "hello")); //<-하나의 데이터라고 보면 됨
	test.insert(make_pair(1, "world")); //저장하는 방법은 insert밖에 없음 //리턴값은 true, false인데 iterator와 동시에 반환한다 //키값이 중복되지 않는다면 들어간다
	test.insert(make_pair(1, "good job")); // <= 키값이 중복되서 데이터 저장이 안 되었다.
	*/

	map<const char*, const char*> test;

	test.insert(make_pair("hello", "hello"));
	test.insert(make_pair("world", "world"));
	test.insert(make_pair("mi", "good job"));
	

	//map<int, const char*>::iterator it;

	/*
	for(int i=0; i<10; i++)
	{
		it = test.find(i); //먼저 값을 찾아보고
		if(it != test.end()) //없는 걸 찾으면 end를 반환
			cout << it->first << " : " << it->second << endl;  //pair은 퍼스트랑 세컨드로 나눠져 있다
		else
			cout << "없다" << endl;
	}
	*/

	//cout << test[0] << endl; //test[0] : 0키값을 가리키는 녀석을 말한다


	//원래대로 할려면
	map<const char*, const char*>::iterator it;
	it = test.find("hello");
	it->second = "bye";

	cout << test["hello"] << endl;

	test["hello"] = "byebye"; //이렇게하면 쉽게 데이터를 바꿀 수 있다.

	cout << test["hello"] << endl;

	return 0;
}