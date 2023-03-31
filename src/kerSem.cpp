#include "kerSem.h"
#include <iostream.h>

extern int lck;
extern int request;
extern void dispatch ();

#define LOCK      ++lck
#define UNLOCK    if (--lck == 0 && request) { dispatch(); }

KernelSem::KernelSem(Semaphore* sem, int v) {
	value=v;
	MySem=sem;
	ListOfBlkThr=new List();
}

KernelSem::~KernelSem() {
	ListOfBlkThr->putListToShedule();
	delete ListOfBlkThr;
}

int KernelSem::val() const{
	return value;
}

int KernelSem::wait (Time maxTimeToWait){
	LOCK;
		if(value-->0){
			return 1;
		} else{
			if(maxTimeToWait > 0){
				PCB::BlockedPCBs->insert((PCB*) PCB::running, maxTimeToWait);
				PCB::running->mySem= this->MySem;
			}
			ListOfBlkThr->insert((PCB*) PCB::running);
			PCB::running->state = BLOCKED;

		UNLOCK;

		dispatch();

		LOCK;

		if(PCB::running->mySem == 0 ){
			UNLOCK;
			return 1;
		} else{
			PCB::running->mySem = 0;
			UNLOCK;
			return 0;
		}

	}

}

void KernelSem::signal(){
	LOCK;
		if(value  < 0){
				PCB* DeBlockedThr = ListOfBlkThr->takeFirst();
				DeBlockedThr->state=READY;
				DeBlockedThr->mySem=0;
				PCB::BlockedPCBs->remove(DeBlockedThr->id);
				Scheduler::put(DeBlockedThr);
		}
		value++;
	UNLOCK;
}
