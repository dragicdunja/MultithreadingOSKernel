/*
 * pubThr.h
 *
 *  Created on: Aug 21, 2021
 *      Author: OS1
 */

#ifndef PUBTHR_H_
#define PUBTHR_H_

#include "thread.h"

class PublicThread: public Thread{
public:

	PublicThread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);

};

PublicThread::PublicThread(StackSize stackSize, Time timeSlice): Thread(stackSize, timeSlice)  {}





#endif /* PUBTHR_H_ */
