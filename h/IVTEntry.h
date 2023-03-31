#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#define PREPAREENTRY(numEntry, callOldIndicator)\
void interrupt inter##numEntry(...); \
\
IVTEntry newEntry##numEntry(numEntry, inter##numEntry, callOldIndicator);\
\
void interrupt inter##numEntry(...) {\
	newEntry##numEntry.signal();\
	if (callOldIndicator == 1)\
		newEntry##numEntry.oldRutine();\
}

typedef unsigned char IVTNo;
typedef void interrupt (*PtrInt)(...);



class IVTEntry {
	friend class KernelEv;
public:
	volatile int callOldIndicator;
	PtrInt newRutine, oldRutine;

	IVTEntry(IVTNo ivtNo, PtrInt rutine, int callOldInd);

	virtual ~IVTEntry();

	void signal();

	void callOldRutine();

	static IVTEntry* EntryNumber(int i);

private:
	static IVTEntry* EntryList[256];

	KernelEv* event;
	volatile int NumberOfEntry;

};

#endif
