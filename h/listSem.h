/*
 * listSem.h
 *
 *  Created on: Aug 23, 2021
 *      Author: OS1
 */

#ifndef LISTSEM_H_
#define LISTSEM_H_

#include "kernSem.h"

class ListSem{
public:

	struct Elem{
			KernelSem* semaphore;
			Elem* next;
			Elem(KernelSem* ks){
				semaphore = ks;
				next = 0;
			}
		};

	Elem* head;
	Elem* tail;


	ListSem();
	~ListSem();
	void add(KernelSem* ks);
	void deleteSem(KernelSem* pcb);
	void checkSemaphores();
	void updateSemValue(int id);

	int sizeSem();

	void printAll();

};




#endif /* LISTSEM_H_ */
