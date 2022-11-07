/*
 * listSem.cpp
 *
 *  Created on: Aug 23, 2021
 *      Author: OS1
 */

#include "listSem.h"


ListSem::ListSem(){

		head = 0;
		tail = 0;
}

ListSem::~ListSem(){

	while(head){
		Elem* old = head;
		head = head->next;
		delete old;
	}

}

void ListSem::add(KernelSem* sem){

	Elem* newSemaphore = new Elem(sem);

	if(!head){
		head = newSemaphore;
		tail = newSemaphore;
	}else{
		tail->next = newSemaphore;
		tail = newSemaphore;
	}

}

void ListSem::deleteSem(KernelSem* sem){

	Elem* current;
	Elem* previous = 0;
	for(current = head; current; current = current->next){
		if(current->semaphore->myID == sem->myID){ // zbog ovoga sa dodala Id u KernelSem
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

void ListSem::checkSemaphores(){

	if(!head){
		return;
	}else{
		Elem* current;
		for(current = head; current; current = current->next){
			if(current->semaphore->blocked != 0)
				current->semaphore->blocked->checkThreadsOnSem(current->semaphore->myID);
		}
	}

}

void ListSem::updateSemValue(int id){

	Elem* current = head;
	while(current){
		if(current->semaphore->myID == id){
			current->semaphore->value++;
			return;
		}
		current = current->next;
	}
}

int ListSem::sizeSem(){

	int cnt = 0;
	Elem* current;
	for(current = head; current; current = current->next){
		cnt++;
	}
	return cnt;

}




