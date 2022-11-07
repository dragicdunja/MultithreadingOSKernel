/*
 * ivtEntry.h
 *
 *  Created on: Sep 9, 2021
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "event.h"
class KernelEv;

typedef void interrupt (*interruptRoutine)(...);

class IVTEntry {
public:

	IVTEntry(IVTNo ivtNo, interruptRoutine newRoutine);
	~IVTEntry();

	interruptRoutine oldRoutine;
	IVTNo entry;
	KernelEv* myEvent;

	void signal();
	void callOldRoutine();
	void setMyEvent(KernelEv* event);

	static IVTEntry* ivts[256];
};



#endif /* IVTENTRY_H_ */
