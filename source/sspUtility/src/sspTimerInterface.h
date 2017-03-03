//////////////////////////////////////
// sspTimerInterface.h : header file
//////////////////////////////////////

#ifndef SSPTIMERINTERFACE_H
#define SSPTIMERINTERFACE_H

#include "sspTime.h"

class sspTimerObject
{
public:
	virtual	bool begin(sspTimerType& nTime) = 0;
	virtual bool update(sspTimerType& nTime) = 0;
	virtual void end() = 0;
};

#endif