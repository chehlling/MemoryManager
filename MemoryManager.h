#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<list>
#include<assert.h>
#include<stdarg.h>
#include"TypeTraits.hpp"
using namespace std;

// ����������
class SaveAdapter
{
public:
	virtual void Save(const char* fmt, ...) = 0;
};

class ConsoleSaveAdapter :public SaveAdapter
{
public:
	virtual void Save(const char* fmt, ...)
	{
		va_list arg;
		va_start(arg, fmt);
		//int vfprintf ( FILE * stream, const char * format, va_list arg );
		vfprintf(stdout, fmt, arg);
		va_end(arg);
	}
};
class FileSaveAdapter :public SaveAdapter
{
public:
	FileSaveAdapter(const char* filename = "memoryLeakReport.txt")
	{
		_fptr = fopen(filename, "w");
	}
	~FileSaveAdapter()
	{
		if (_fptr)
		{
			fclose(_fptr);
		}
	}
	virtual void Save(const char* fmt, ...)
	{
		if (_fptr)
		{
			va_list arg;
			va_start(arg, fmt);
			vfprintf(_fptr, fmt, arg);
			va_end(arg);
		}
	}
protected:
	FileSaveAdapter(const FileSaveAdapter& f);
	FileSaveAdapter& operator=(FileSaveAdapter& f);
protected:
	FILE* _fptr;
};

struct BlockInfo //�ڴ����Ϣ
{
	void* _ptr;
	string _file;
	int _line;
	BlockInfo(void* ptr, string file, int line)
		:_ptr(ptr)
		, _file(file)
		, _line(line)
	{}
};


//����ģʽ
//��װ
//��֤ȫ����ֻ��һ��ʵ������
//�����ױ��������ȡ�����ʵ��
class Singleton
{
public:
	static Singleton* GetInstance();
	void* Alloc(size_t size, string file, int line);//����ռ�
	void Dealloc(void* ptr);//�ͷſռ�

	//�־û�
	static void Persistent();
	void _Persistent(SaveAdapter* sa);//��ӡ�ڴ�й©���ڴ����Ϣ
private:
	Singleton(){};
	Singleton(const Singleton & s);
	Singleton& operator=(const Singleton &s);
protected:
	list<BlockInfo> _blockList;
	static Singleton* sInstance;
};

#define NEW(type) \
	__NEW<type>(sizeof(type), __FILE__, __LINE__)
#define DELETE(ptr) \
	__DELETE(ptr)
#define NEW_ARRAY(type,num) \
	__NEW_ARRAY<type>(sizeof(type)*num, __FILE__, __LINE__)
#define DELETE_ARRAY(ptr)\
	__DELETE_ARRAY(ptr)


template<class T>
T* __NEW(size_t size, const char* file, int line)
{
	T* ptr = (T*)Singleton::GetInstance()->Alloc(size, file, line);
	//new��λ���ʽ���ù��캯����ʼ��
	return new(ptr)T;
}

template<class T>
void __DELETE(T* ptr)
{
	ptr->~T();
	Singleton::GetInstance()->Dealloc(ptr);
}

template<class T>
T* __NEW_ARRAY(size_t size, const char*file, int line)
{
	bool isPodType = TypeTraits<T>::__IsPODType().Get();
	if (isPodType)
	{
		T* ptr = (T*)Singleton::GetInstance()->Alloc(size, file, line);
		return ptr;
	}
	else
	{
		T* p = (T*)Singleton::GetInstance()->Alloc(size + 4, file, line);
		int num = size / sizeof(T);
		*(int*)p = num;
		T* ptr = (T*)((int*)p + 1);
		//����new�Ķ�λ���ʽ���ù��캯��ʵ��������
		T* tmp = ptr;
		while (num--)
		{
			new(tmp)T;
			tmp++;
		}
		return ptr;
	}
}

template<class T>
void __DELETE_ARRAY(T *ptr)
{
	bool isPodType = TypeTraits<T>::__IsPODType().Get();
	if (isPodType)
	{
		Singleton::GetInstance()->Dealloc(ptr);
	}
	else
	{
		T* tmp = ptr;
		int* tmp1 = (int*)ptr;
		int num = *(--tmp1);
		//��ʾ�ĵ���num����������
		while (num--)
		{
			tmp->~T();
			tmp++;
		}
		Singleton::GetInstance()->Dealloc(tmp1);
	}
}