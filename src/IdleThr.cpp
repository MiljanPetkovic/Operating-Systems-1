#include <iostream.h>
#include "IdleThr.h"

extern int lck;
extern int request;
extern void dispatch ();

#define LOCK      ++lck
#define UNLOCK    if (--lck == 0 && request) { dispatch(); }

IdleThr::IdleThr() : Thread(defaultStackSize, 1){}

IdleThr::~IdleThr() {}

void IdleThr::run() {
	for (int i = 0; i < 10; i++) {
/*		LOCK;
		cout<<"idle\n";
		UNLOCK;*/
		for (int j = 0; j < 30000; j++)
			for (int k = 0; k < 30000; k++);
		if (i == 9) i = -1;
	}
}

