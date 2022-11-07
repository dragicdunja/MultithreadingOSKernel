/*
 * kernSem.h
 *
 *  Created on: Aug 23, 2021
 *      Author: OS1
 */

#ifndef KERNSEM_H_
#define KERNSEM_H_

#include "semaphor.h"
#include "list.h"

class Lista;

class KernelSem{

public:

	KernelSem(Semaphore* semaphore, int init);
	~KernelSem();

	int wait (Time maxTimeToWait);
	void signal();

	int value;
	Semaphore* mySemaphore;
	Lista* blocked; // lista niti koje cekaju na ovom semaforu

	int getValue();
	int block(Time maxTimeToWait);
	void deblock();

	static int id;
	int myID;

};




#endif /* KERNSEM_H_ */
