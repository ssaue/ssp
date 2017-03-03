// inpout32drv.h : Defines the interface for the host application.
//

#ifndef INPOUT32DRV_H
#define INPOUT32DRV_H

void  _stdcall Out32(short PortAddress, short data);
short _stdcall Inp32(short PortAddress);

#endif
