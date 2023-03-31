#include "IVTEntry.h"
#include <dos.h>
#include "kerEvent.h"

IVTEntry* IVTEntry::EntryList[256] = { 0 };

IVTEntry::IVTEntry(IVTNo ivtNo, PtrInt rutine, int callOldInd) {
	newRutine=rutine;
	NumberOfEntry=ivtNo;
#ifndef BCC_BLOCK_IGNORE
	oldRutine = getvect(NumberOfEntry);
	setvect(NumberOfEntry, newRutine);
#endif
	callOldIndicator=callOldInd;
	event=0;
	IVTEntry::EntryList[NumberOfEntry] = this;
}

IVTEntry::~IVTEntry() {
	IVTEntry::EntryList[NumberOfEntry] = 0;
	#ifndef BCC_BLOCK_IGNORE
		setvect(NumberOfEntry, oldRutine);
	#endif
}

void IVTEntry::signal() {
	event->signal();
}

void IVTEntry::callOldRutine() {
	oldRutine();
}

IVTEntry* IVTEntry::EntryNumber(int i) {
	return IVTEntry::EntryList[i];
}
