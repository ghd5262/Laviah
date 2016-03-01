#pragma once
#include "..\Common\HSHUtility.h"

class CTask;
class CTaskList;
class CTaskIter;

// ==============================================================
// �۾�
class CTask {
	friend CTaskList;
	friend CTaskIter;

private:
	CTask* Prev;
	CTask* Next;

	void* operator new (size_t n){}
	void operator delete (void* p) {}

public:
	CTask(){}
	CTask(const CTaskList* list);
	virtual ~CTask();
};

// ==============================================================
// �۾� ���
class CTaskList {
	friend CTask;
	friend CTaskIter;

private:
	CTask* ActiveTask;
	CTask* FreeTask;
	size_t TaskSize;
	size_t TaskCount;
	char* Buffer;

	CTask* GetTask(size_t i);

public:
	CTaskList(size_t size, size_t count);
	~CTaskList();

	void* New(size_t size);
	void Delete(void* p);
};

// ==============================================================
// �۾� �ݺ���
class CTaskIter {

private:
	CTaskList* List;
	CTask* Task;

public:
	CTaskIter(CTaskList* list);
	bool HasNext();
	CTask* Next();
	void Remove();
};