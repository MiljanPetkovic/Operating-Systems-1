#include "kerEvent.h"
#include "pcb.h"
#include "IVTEntry.h"
#include <iostream.h>

extern int lck;
extern int request;
extern void dispatch ();

#define LOCK      ++lck
#define UNLOCK    if (--lck == 0 && request) { dispatch(); }

KernelEv::KernelEv(IVTNo ivtNo, Event* event){
	myEvent = event;
	myPCB=(PCB*)PCB::running;
	IVTEntry::EntryNumber(ivtNo)->event = this;
	value = 0;
}


KernelEv::~KernelEv() {
	myPCB = 0;
	myEvent = 0;
}

void KernelEv::signal() {
	LOCK;

	if(myPCB->state == BLOCKED){
			myPCB->state = READY;
			Scheduler::put(myPCB);
	}
	else value = 1;

	UNLOCK;
}

void KernelEv::wait() {
	if(myPCB != PCB::running) return;



	if(value == 0){
			PCB::running->state = BLOCKED;
			UNLOCK;
			dispatch();
	} else
		value = 0;
}
