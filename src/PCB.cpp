#include "PCB.h"
#include <dos.h>
#include "List.h"
#include "System.h"

ID PCB::nextId = -2;
volatile PCB* PCB::running = 0;
List* PCB::allPCBs=new List();
List* PCB::BlockedPCBs=new List();

extern unsigned lck;
extern unsigned request;

#define LOCK      ++lck
#define UNLOCK    if (--lck == 0 && request) { dispatch(); }

PCB::PCB(Thread* _myThread,	StackSize _stackSize,	Time _timeSlice){
	id = nextId++;
	myThread = _myThread;
	timeSlice = _timeSlice;
	mySem=0;
	state = CREATED;

	stackSize = _stackSize;
	if (stackSize > 65536) stackSize = 65536;
	stackSize /= sizeof(unsigned);

	stack = new unsigned[stackSize];
	stack[stackSize - 1] = 0x200;

#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 2] = FP_SEG(wrapperRun);
	stack[stackSize - 3] = FP_OFF(wrapperRun);
	ss = FP_SEG(stack + stackSize - 12);
	sp = FP_OFF(stack + stackSize - 12);
	bp = sp;
	#endif

	waitingList = new List();
	allPCBs->insert(this);
}

PCB::~PCB() {
	delete stack;
	delete waitingList;
}

void PCB::start(){
	state = READY;
	Scheduler::put(this);
}

void PCB::waitToComplete(){
	if (this == running || this->state == FINISHED  || this == System::idle )
		return;

	LOCK;
	running->state = BLOCKED;
	waitingList->insert((PCB*)running);
	UNLOCK;
	dispatch();
}

Thread* PCB::getThread(ID id) {
	return allPCBs->getThreadById(id);
}


void PCB::wrapperRun() {

	running->myThread->run();

	LOCK;
	running->state = FINISHED;
	running->waitingList->putListToShedule();
	UNLOCK;

	System::dispatch();

}


