#include "event.h"
#include "kerEvent.h"

extern int lck;
extern int request;
extern void dispatch ();

#define LOCK      ++lck
#define UNLOCK    if (--lck == 0 && request) { dispatch(); }

Event::Event(IVTNo ivtNo) {
	LOCK;
	myImpl = new KernelEv(ivtNo, this);
	UNLOCK;
}

Event::~Event() {
	delete myImpl;
}

void Event::wait() {
	myImpl->wait();
}

void Event::signal() {
	myImpl->signal();
}

