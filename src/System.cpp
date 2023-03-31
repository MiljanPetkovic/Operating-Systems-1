#include "System.h"
#include "PCB.h"
#include "List.h"


volatile unsigned lck=0;
volatile unsigned request=0;

volatile PCB* mainThread;
volatile Time cntr;

IdleThr* System::idle = 0;

unsigned tss;
unsigned tsp;
unsigned tbp;

typedef void interrupt (*PtrInt)(...);
PtrInt oldTimer;

System::System() { }

System::~System() {}

void System::initialize() {
	asm cli;
		#ifndef BCC_BLOCK_IGNORE
			oldTimer = getvect(0x08);
			setvect(0x08, timer);
			setvect(0x60, oldTimer);
		#endif

		idle = new IdleThr();

		PCB::running =  new PCB(0, 0, defaultTimeSlice);
		mainThread = (PCB*)PCB::running;
		PCB::running->state = RUN;
	asm sti;
}

void System::restore(){
	asm cli;
		mainThread->state =  FINISHED;
		delete mainThread;

		idle->myPCB->state =  FINISHED;
		delete idle;

		#ifndef BCC_BLOCK_IGNORE
			setvect(0x08, oldTimer);
		#endif
	asm sti;
}

extern void tick();

void interrupt System::timer(...){
	if(!request && cntr)
		cntr--;

	if(!request){
		tick();
		asm int 60h;
		//Odblokiravanje zablokiranih nitu
		PCB::BlockedPCBs->putSemBlockedToShedule();
	}

	if (  cntr == 0  || request) {
		request=0;
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		if( (PCB::running->state==RUN) && (PCB::running != idle->myPCB)){
			Scheduler::put((PCB*)PCB::running);
			PCB::running->state = READY;
		}

		PCB::running=Scheduler::get();

		if (PCB::running == 0)
				PCB::running = idle->myPCB;

		PCB::running->state = RUN;

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		cntr = PCB::running->timeSlice;

		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp, tbp
		}
	}
	// poziv stare prekidne rutine koja se
     // nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena
     // konteksta – tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida
}

void System::dispatch(){ // sinhrona promena konteksta
	asm cli;
		request=1;
		timer();
	asm sti
}
