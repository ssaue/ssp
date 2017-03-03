// SSpBasic.h: basic interfaces for the SoundSpace project
//
//////////////////////////////////////////////////////////////////////

#ifndef SSP_BASIC_H
#define SSP_BASIC_H 1

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SSpObject.h"
#include <string>

class sspValue : public sspObject  
{
public:
	sspValue(short nType) : sspObject(SSP_CLASS_VALUE, nType) {}
	virtual float getValue() = 0;
};

class sspConditional : public sspObject  
{
public:
	sspConditional(short nType) : sspObject(SSP_CLASS_CONDITIONAL, nType) {}
	virtual bool initialize() {return true;}
	virtual bool isTrue() = 0;
};

class sspStringObject : public sspObject  
{
public:
	sspStringObject(short nType) : sspObject(SSP_CLASS_STRING, nType) {}
	virtual sspString getString() = 0;
};

#endif
