/*
 * global.cpp
 *
 *  Created on: Aug 21, 2021
 *      Author: OS1
 */

#include "global.h"
#include "semaphor.h"


volatile unsigned tsp;
volatile unsigned tss;
volatile unsigned tbp;
volatile unsigned oldTimerSEG;
volatile unsigned oldTimerOFF;


void interrupt timer(){

	if(!contextSwitch){
		tick();
		if(allSems != 0 && lockSem == 1)
			allSems->checkSemaphores();
		asm int 60h; //stvarno je dosao prekid od tajmera i pozivamo staru prekidnu rutinu koja je na 60h, a ranije je bila na 08h
	}
	if (!contextSwitch && running->timeSlice != 0 ){
		counter--;
	}

	if ((counter == 0 && running->timeSlice != 0) || contextSwitch) {
		if(lock == 0 || lockSem == 0){

			contextSwitch = 1;

		}else{

			contextSwitch = 0;

			asm {
						// cuva sp
						mov tsp, sp
						mov tss, ss
						mov tbp, bp
					}

					running->sp = tsp;
					running->ss = tss;
					running->bp = tbp;

					if(running->state != PCB::FINISHED && running->state != PCB::BLOCKED && running->threadID != idleThread->getId()){
						Scheduler::put((PCB*)running);
					}

					running = Scheduler::get();

					if(running == 0){ // znaci nema nista u Scheduler-u (sve niti su blokirane) i onda mora da radi idle
						running = idleThread->getPCB();
					}

					tsp = running->sp;
					tss = running->ss;
					tbp = running->bp;

					counter = running->timeSlice;

					asm {
						mov sp, tsp
						mov ss, tss
						mov bp, tbp
					}

		}

	}

}

void dispatch(){
	if(newIntr){
		asm cli;
		contextSwitch = 1;
		timer();
		asm sti;
	}
}

void inic(){
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

// vraca staru prekidnu rutinu
void restore(){
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}


int main(int argc, char* argv[]){

	allPCBs = new Lista();
	flagIdle = 1;
	idleThread = new Idle();
	mainThread = new PublicThread();
	mainThread->getPCB()->state = PCB::READY;
	running = mainThread->getPCB();

	inic();

	newIntr = 1;

	int ret;
	ret = userMain(argc, argv);

	restore();

	newIntr = 0;

	delete allPCBs;
	delete allSems;
	delete idleThread;
	delete mainThread;

	return ret;

}





