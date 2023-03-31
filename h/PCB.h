#ifndef pcb_h_
#define pcb_h_

#include "Thread.h"
#include "SCHEDULE.H"

class List;
class Semaphore;

enum State {
	CREATED = 0,
	READY,
	RUN,
	BLOCKED,
	FINISHED
};

class PCB{
public:
	ID id;
	static volatile PCB* running;
	static List* BlockedPCBs;
	Semaphore* mySem;
	State state;

private:
	unsigned *stack;
	StackSize stackSize;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	Time timeSlice;
	Thread* myThread;
	List* waitingList;

	PCB(Thread* _myThread,	StackSize _stackSize,	Time _timeSlice);

	virtual ~PCB();

	void start();

	void waitToComplete();//nije zavrseno

	//ID counter
	static ID nextId;

	static List* allPCBs;


	static Thread* getThread(ID id);

	static void wrapperRun();//nije zavresno


	friend class System;
	friend class List;
	friend class Thread;
};


#endif
