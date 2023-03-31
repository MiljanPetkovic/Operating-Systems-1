#ifndef KERSEM_H_
#define KERSEM_H_
#include "semaphor.h"
#include "List.h"

class KernelSem {
private:
	int value;
	List* ListOfBlkThr;
	Semaphore* MySem;

	KernelSem(Semaphore* sem, int v);

	virtual ~KernelSem();

	int val() const;

	virtual int wait (Time maxTimeToWait);

	virtual void signal();

	friend class Semaphore;
	friend class List;
};

#endif
