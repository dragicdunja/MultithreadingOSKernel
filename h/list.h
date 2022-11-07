/*
 * list.h
 *
 *  Created on: Aug 21, 2021
 *      Author: OS1
 */

#ifndef LIST_H_
#define LIST_H_

#include "pcb.h"
#include "kernSem.h"

class ListSem;

class Lista{
public:

	struct Elem{
			PCB* pcb;
			Elem* next;
			Elem(PCB* p){
				pcb = p;
				next = 0;
			}
		};

	Elem* head;
	Elem* tail;


	Lista();
	~Lista();
	int size();
	void add(PCB* pcb);
	void release();
	Thread* getPCBbyID(ID id);
	void deletePCB(PCB* pcb);

	PCB* takeFirst();
	void checkThreadsOnSem(int id);

	void printAll();

};




#endif /* LIST_H_ */
