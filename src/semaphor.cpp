/*
 * semaphor.cpp
 *
 *  Created on: Aug 23, 2021
 *      Author: OS1
 */

#include "semaphor.h"
#include "kernSem.h"

extern volatile unsigned lock;
extern int contextSwitch;

void dispatch();

Semaphore::Semaphore(int init){

	lock = 0;
	int value = init;
	if(value < 0){
		value = 0;
	}
	myImpl = new KernelSem(this, value);
	lock = 1;
	if(contextSwitch){
		dispatch();
	}

}

Semaphore::~Semaphore(){

	lock = 0;
	delete myImpl;
	lock = 1;
	if(contextSwitch){
		dispatch();
	}

}

int Semaphore::wait(Time maxTimeToWait){

	int ret = 0;
	if(myImpl != 0)
		ret = myImpl->wait(maxTimeToWait);
	return ret;

}

void Semaphore::signal(){

	if(myImpl != 0)
		myImpl->signal();

}

int Semaphore::val() const{

	if(myImpl != 0){
		return myImpl->getValue();
	}else{
		return 0;
	}

}

KernelSem* Semaphore::getKernelSem(){

	return myImpl;

}



