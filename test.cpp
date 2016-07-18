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
	// �Ǽ�һ��������main����ִ�н����Ժ�ִ��
	atexit(&Singleton::Persistent);

	Test1();
	system("pause");
	return 0;
}
//new�Ķ�λ���ʽ
//�ɱ�����б�
//������