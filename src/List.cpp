#include "List.h"

extern int lck;
extern int request;
extern void dispatch ();

#define LOCK      ++lck
#define UNLOCK    if (--lck == 0 && request) { dispatch(); }

List::List() {
	tail = head = 0;
}

void List::insert(PCB* pcb) {
	Elem* newly = new Elem(pcb);
	if (!head)
		tail = head = newly;
	else
		tail = tail->next = newly;
}

void List::insert(PCB* pcb, int time){
	Elem* newly = new Elem(pcb);

	LOCK;
	if (!head)
		tail = head = newly;
	else {
		if (time < head->time) {
			head->time = head->time - time;
			newly->next = head;
			head = newly;

		}
		else {
			Elem* tmp = head;
			time -= tmp->time;
			while (tmp != tail) {
				if (tmp->next->time <= time) {
					tmp = tmp->next;
					time -= tmp->time;
				}
				else {
					tmp->next->time -= time;
					break;
				}
			}
			if(tmp == tail )
				tail=newly;
			newly->next = tmp->next;
			tmp->next = newly;
		}
	}
	newly->time = time;
	UNLOCK;
}

void List::decTime() {
	if (!head ) return;
	if (head->time == 0) return;
	head->time--;
}


void List::remove(ID id) {
	Elem* prev = 0, *tmp = head;

	while (tmp && tmp->pcb->id != id) {
		prev = tmp;
		tmp = tmp->next;
	}

	if (!tmp) return;

	if (prev){
		prev->next = tmp->next;
		tmp->next=0;
		if(tail==tmp)
			tail=prev;
	}
	else{
		head = tmp->next;
		tmp->next=0;
	}

	delete tmp;

	if (!head) tail = head;
}

PCB*  List::returnPCB(ID id){
	Elem* tmp = head;

	while (tmp && tmp->pcb->id != id) {
		tmp = tmp->next;
	}

	return tmp->pcb;
}

Thread* List::getThreadById(ID id) {

	Elem* tmp = head;

	while (tmp){
		tmp = tmp->next;
		if (tmp->pcb->id == id)
			break;
	}

	if (!tmp)
		return 0;

	return tmp->pcb->myThread;

}


void List::putListToShedule() {
	while (head) {
		Elem* tmp = head;
		head->pcb->state = READY;
		Scheduler::put(head->pcb);
		head = head->next;
		delete tmp;
	}
	tail = 0;
}


int List::empty(){
	if (!head)
		return 1;
	else
		return 0;
}

int List::idOfFirst(){
	return head->pcb->id;
}

PCB* List::takeFirst(){
	int firstID= idOfFirst();
	PCB* tmp = returnPCB(firstID);
	remove(firstID);
	return tmp;
}

void List::putSemBlockedToShedule(){
	LOCK;
		head->time--;
		while (head && head->time==0) {
			//izbacivanje elementa iz interne liste blokiranih na semaforu
			head->pcb->mySem->myImpl->ListOfBlkThr->remove(head->pcb->id);
			head->pcb->mySem->myImpl->value++;
			// ubacivanje u scheduler
			Elem* tmp = head;
			head->pcb->state = READY;
			Scheduler::put(head->pcb);
			head = head->next;
			//brisanje elsementa
			delete tmp;
			}
		if(!head)
			tail=0;
	UNLOCK;
}


List::~List() {
	if (!head) return;

	while (head) {
		Elem* help = head;
		head = head->next;
		delete help;
	}

	tail = 0;
}

