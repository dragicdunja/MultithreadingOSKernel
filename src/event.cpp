/*
 * event.cpp
 *
 *  Created on: Sep 9, 2021
 *      Author: OS1
 */

#include "event.h"
#include "kernEv.h"

extern volatile unsigned lock;
extern int contextSwitch;

void dispatch ();

Event::Event(IVTNo ivtNo){

	lock = 0;
	myImpl = new KernelEv(ivtNo, this);
	lock = 1;
	if(contextSwitch){
		dispatch();
	}

}

Event::~Event(){

	delete myImpl;

}

void Event::signal(){

	if(myImpl != 0)
		myImpl->signal();

}

void Event::wait(){

	if(myImpl != 0)
		myImpl->wait();

}


