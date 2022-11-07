/*
 * global.h
 *
 *  Created on: Aug 21, 2021
 *      Author: OS1
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "pcb.h"
#include "idle.h"
#include "pubThr.h"
#include "listSem.h"

class ListSem;

volatile int contextSwitch = 0;
volatile int counter = 20;
volatile PCB* running;
Lista* allPCBs;
Thread* idleThread;
Thread* mainThread;
volatile int newIntr = 0;  // promenljiva koja govori da li je nova ili stara prekidna rutina
int flagCallOld = 0;
int flagIdle = 1;

volatile ListSem* allSems = new ListSem();

volatile unsigned lock = 1;
volatile unsigned lockSem = 1;

void dispatch();
void inic();
void restore();

void tick();
int userMain(int argc, char* argv[]);



#endif /* GLOBAL_H_ */
