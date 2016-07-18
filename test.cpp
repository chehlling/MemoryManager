#include"MemoryManager.h"


class AA
{
public:
	AA()
	{
		cout << "A()" << endl;
	}
	~AA()
	{
		cout << "~A()" << endl;
	}
};

void Test1()
{
	AA* p1 = NEW(AA);
	DELETE(p1);

	int* p2 = NEW(int);
	DELETE(p2);

	AA*p3 = NEW_ARRAY(AA, 10);
	DELETE_ARRAY(p3);

	double* p4 = NEW_ARRAY(double, 8);
	DELETE_ARRAY(p4);
}

int main()
{
	// 登记一个函数在main函数执行结束以后执行
	atexit(&Singleton::Persistent);

	Test1();
	system("pause");
	return 0;
}
//new的定位表达式
//可变参数列表
//适配器