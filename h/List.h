#ifndef LIST_H_
#define LIST_H_

#include "pcb.h"
#include "Thread.h"
#include "SCHEDULE.H"
#include "kerSem.h"

class List {
	friend class PCB;

private:
	struct Elem {
		PCB* pcb;
		int time;
		Elem* next;

		Elem(PCB* p, int t = -1, Elem* n = 0) : pcb(p), time(t), next(n) {}

		~Elem() {}
	};
	Elem *head, *tail;

public:
	List();

	virtual ~List();

	void insert(PCB* pcb); // dodavanje u listu koja koja pamti sve pcb, ili ono koji su spremni

	void insert(PCB* pcb, int time); // Dodavanje u listu koja ce da pamti sleep niti

	void putListToShedule();

	void putSemBlockedToShedule();

	int empty();

	int idOfFirst();

	PCB* takeFirst();

	void decTime();

	Thread* getThreadById(ID id);

	void remove(ID id);

	PCB* returnPCB(ID id);
};

#endif
