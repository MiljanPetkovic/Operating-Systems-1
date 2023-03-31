#include "Thread.h"
#include "pcb.h"
#include "System.h"

extern int lck;
extern int request;

void dispatch();
#define LOCK      ++lck
#define UNLOCK    if (--lck == 0 && request) { dispatch(); }


Thread::Thread (StackSize stackSize, Time timeSlice ){
	LOCK;
	myPCB = new PCB(this, stackSize, timeSlice);
	UNLOCK;
}

void Thread::start(){
		myPCB->start();
}

void Thread::waitToComplete() {
	myPCB->waitToComplete();
}

ID Thread::getId() {
	return myPCB->id;
}

ID Thread::getRunningId() {
	return PCB::running->id;
}

Thread* Thread::getThreadById(ID id) {
	return PCB::getThread(id);
}

Thread::~Thread() {
	waitToComplete();
	delete myPCB;
}

void dispatch() {
	System::dispatch();
}


