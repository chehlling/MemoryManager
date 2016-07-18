#include"MemoryManager.h"

void* Singleton::Alloc(size_t size, string file, int line)
{
	void* ptr = malloc(size);
	if (ptr != NULL)
	{
		BlockInfo info(ptr, file, line);
		_blockList.push_back(info);
	}
	return ptr;
}
void Singleton::Dealloc(void* ptr)
{
	free(ptr);
	if (ptr != NULL)
	{
		list<BlockInfo>::iterator it = _blockList.begin();
		while (it != _blockList.end())
		{
			if (it->_ptr == ptr)
			{
				_blockList.erase(it);
				return;
			}
			it++;
		}
		assert(false);
	}
}
void Singleton::_Persistent(SaveAdapter* sa)
{
	list<BlockInfo>::iterator it = Singleton::_blockList.begin();
	while (it != _blockList.end())
	{
		int i = 1;
		//printf("ptr:%p\nfile:%s\nline:%d\n", it->_ptr,it->_file.c_str() ,it->_line);
		sa->Save("¡¾ÄÚ´æÐ¹Â¶%d:¡¿ptr:%p, file:%s, line:%d\n",
			i, it->_ptr, it->_file.c_str(), it->_line);
		it++;
		cout << "\n";
	}
}
void Singleton::Persistent()
{
	ConsoleSaveAdapter csa;
	GetInstance()->_Persistent(&csa);

	FileSaveAdapter fsa;
	GetInstance()->_Persistent(&fsa);
}

Singleton* Singleton::sInstance = NULL;
Singleton* Singleton::GetInstance()
{
	if (sInstance == NULL)
	{
		sInstance = new Singleton;
	}
	return sInstance;
}