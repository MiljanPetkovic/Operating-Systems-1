#include "semaphor.h"
#include "kerSem.h"

Semaphore:: Semaphore (int init){
	myImpl= new KernelSem(this, init);
}

Semaphore::~Semaphore() {
	delete myImpl;
}


int Semaphore::wait (Time maxTimeToWait){
	int tmp = myImpl->wait(maxTimeToWait);
}

void Semaphore::signal(){
	myImpl->signal();
}

int Semaphore::val () const{
	return myImpl->value;
}
