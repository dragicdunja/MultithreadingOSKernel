/*
 * event.h
 *
 *  Created on: Sep 9, 2021
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_



typedef unsigned char IVTNo;
class KernelEv;

extern int flagCallOld;

#include "ivtEntry.h"

class Event {
public:
	 Event (IVTNo ivtNo);
	~Event ();

	 void wait ();

protected:
	 friend class KernelEv;
	 void signal(); // can call KernelEv

private:
 KernelEv* myImpl;
};

#define PREPAREENTRY(ivtNum, callOld)\
		void interrupt newInter##ivtNum(...); \
		IVTEntry newEntry##ivtNum(ivtNum, newInter##ivtNum); \
		void interrupt newInter##ivtNum(...) {\
			flagCallOld = callOld;\
			newEntry##ivtNum.signal();\
			if (callOld == 1)\
				newEntry##ivtNum.callOldRoutine();\
		}

#endif /* EVENT_H_ */
