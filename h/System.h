#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <dos.h>
#include "idleThr.h"

class PCB;

class System {
public:
	System();

	virtual ~System();

	static void initialize();

	static void restore();

	static void interrupt System::timer(...);

	static void dispatch();

	static IdleThr* idle;
};

#endif
