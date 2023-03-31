#ifndef KEREVENT_H_
#define KEREVENT_H_

typedef unsigned char IVTNo;
class PCB;

class KernelEv {
	friend class Event;
	friend class IVTEntry;
private:
	Event* myEvent;
	PCB* myPCB;
	int value;

	KernelEv(IVTNo ivtNo, Event* event);

	virtual ~KernelEv();

	void wait ();

	void signal();


};

#endif
