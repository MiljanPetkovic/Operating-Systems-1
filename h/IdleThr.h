#ifndef IDLETHR_H_
#define IDLETHR_H_

#include "Thread.h"

class IdleThr: public Thread{
public:

	IdleThr();

	virtual ~IdleThr();

	void run();

	friend class System;
};

#endif
