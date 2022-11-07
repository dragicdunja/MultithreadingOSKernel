/*
 * ivtEntry.cpp
 *
 *  Created on: Sep 9, 2021
 *      Author: OS1
 */

#include "ivtEntry.h"
#include <dos.h>
#include "kernEv.h"

extern int flagCallOld;

IVTEntry* IVTEntry::ivts[256]={0};

IVTEntry::IVTEntry(IVTNo ivtNo, interruptRoutine newRoutine){

	entry = ivtNo;
	myEvent = 0;
#ifndef BCC_BLOCK_IGNORE
	asm cli
	oldRoutine = getvect(entry);
	setvect(entry, newRoutine);
	asm sti
#endif
	ivts[entry] = this;

}

IVTEntry::~IVTEntry(){

	callOldRoutine();

#ifndef BCC_BLOCK_IGNORE
	asm cli
	setvect(entry, oldRoutine);
	asm sti
#endif
	ivts[entry] = 0;
	myEvent = 0;

}

void IVTEntry::signal(){

	if(myEvent != 0){
		myEvent->signal();
	}else{
		if(flagCallOld == 0){
			callOldRoutine();
		}
	}

}

void IVTEntry::callOldRoutine(){

	(*oldRoutine)();

}

void IVTEntry::setMyEvent(KernelEv* event){

	myEvent = event;

}


