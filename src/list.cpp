/*
 * list.cpp
 *
 *  Created on: Aug 21, 2021
 *      Author: OS1
 */

#include "list.h"
#include "listSem.h"

extern ListSem* allSems;

Lista::Lista(){

		head = 0;
		tail = 0;
}

Lista::~Lista(){

	while(head){
		Elem* old = head;
		head = head->next;
		delete old;
	}

}

int Lista::size(){
	int cnt = 0;
	Elem* current;
	for(current = head; current; current = current->next){
		cnt++;
	}
	return cnt;
}

void Lista::add(PCB* pcb){

	Elem* newPCB = new Elem(pcb);

	if(!head){
		head = newPCB;
		tail = newPCB;
	}else{
		tail->next = newPCB;
		tail = newPCB;
	}

}

void Lista::release(){

	while(head){
		Elem* old = head;
		head = head->next;
		old->pcb->state = PCB::READY;
		Scheduler::put(old->pcb);
		//brisanje
	}

}

Thread* Lista::getPCBbyID(ID id){

	Elem* current = head;
	while(current){
		if(current->pcb->threadID == id){
			return current->pcb->myThread;
		}
		current = current->next;
	}
	return 0;

}

void Lista::deletePCB(PCB* pcb){

	Elem* current;
	Elem* previous = 0;
	for(current = head; current; current = current->next){
		if(current->pcb->threadID == pcb->threadID){
			if(previous != 0){
				previous->next = current->next;
			}else{
				head = head->next;
			}
			if(current == tail){
				tail = previous;
			}
			//brisanje
			return;
		}
		previous = current;
	}

}

//SEMAFORI


PCB* Lista::takeFirst(){

	Elem* oldHead = head;
	head = head->next;
	PCB* ret = oldHead->pcb;
	ret->state = PCB::READY;
	Scheduler::put(ret);
	//brisanje
	return ret;

}

void Lista::checkThreadsOnSem(int id){

	if(!head){ //niko nije blokiran na ovom semaforu
		return;
	}else{
		Elem* current = head;
		Elem* previous = 0;
		Elem* old;
		while(current){

			if(current->pcb->waitingTime != 0){
				current->pcb->timeLeft = current->pcb->timeLeft - 1;
			}

			if(current->pcb->timeLeft == 0 && current->pcb->waitingTime != 0){

				old = current;

				current->pcb->state = PCB::READY;
				current->pcb->signalHappened = 0;
				Scheduler::put(current->pcb);

				if(allSems != 0)
					allSems->updateSemValue(id);

				if(previous != 0){ // nije prvi obrisan
					previous->next = current->next;
				}else{
					head = head->next;
				}
				if(current == tail){
					tail = previous;
				}
				//brisanje

			}
			previous = current;
			current  = current->next;
		}
	}
}


