/*************************************************************************************/
/*                 UART.DLL                                                          */
/* The UART.DLL si the dynamic linking library(DLL)designed for Windows 95/98        */
/* NT 3.51/4.0/2000/XP applications. The users can apply it to develop their own     */
/* serial COM port program through many programming languages such as VC++,BC++      */
/* Builder,VB and Delphi.                                                            */
/*									                                                 */
/*  Please refer to the DCON_DLL.pdf located                                         */
/*  for more detail regarding the Uart.dll functions                                 */
/*													                                 */
/* Modification History:					                                         */
/*		Refer to version.txt for more details										 */
/*																					 */
/* v2.1.2[9/22/2004] by sean												         */
/*			New:																	 */
/*				1.Add Mark,Space parity ['04/09/17]									 */
/*				2.Add predefinition to avoid to use GetOverlappedResult().			 */
/*				3.Add the function ReadComn().										 */
/* v2.1.1[7/15/2004] by sean														 */
/*          Thread/No Thread option													 */
/* v2.1.0[4/7/2004] by Sean															 */
/*			New: Support the thread fucntion ['04/4/7]								 */
/* v2.0.5[2/2/2004] by Sean															 */
/*			New: 1. Add the function Set_Break ['04/2/2]							 */
/* v2.04[Sep/19/2003] by sean			                                             */
/*				Modify the source code for Miscosoft OS and WinCE OS                 */
/* v2.03[Jun/01/2003] by sean														 */
/*				New: DataSizeInCom,SetLineStatus, GetLineStatus, DataSizeOutCom      */
/*				Modify: Receive_Binary,vxc_write									 */
/*				Fix:  Fix the input limitation to exceed 1024 bytes					 */
/* V2.02[2003] by Sean																 */
/*				New: Send_Receive_Cmd_WithChar,Send_Cmd_WithChar,Receive_Cmd_WithChar*/
/*     				 and Receive_Binary_MOS() 										 */
/* V2.00[Mar/17/2002] by Kevin														 */
/*				Refer to version.txt for more details                                */	
/*************************************************************************************/

#ifdef _WIN32_WCE   //Provided for WinCon
	#define EXPORTS UARTCE_API
//	#define CALLBACK ""
	#include <UARTCE.h>
//	#ifndef __UART_H__
//	#define __UART_H__	

#else
//#ifdef WIN32 //Provided for PC (Microsoft OS)
	#ifndef EXPORTS
		#ifdef __cplusplus
			#define EXPORTS extern "C" __declspec(dllimport)
		#else
			#define EXPORTS __declspec(dllimport)
		#endif
	#endif
#endif

//#define EXPORTS

#define    Data5Bit                     5
#define    Data6Bit                     6
#define    Data7Bit                     7
#define    Data8Bit                     8

#define    NonParity                    0
#define    OddParity                    1
#define    EvenParity                   2

#define    OneStopBit                   0
#define    One5StopBit                  1
#define    TwoStopBit                   2

// return code
#define    NoError                      0
#define    FunctionError                1
#define    PortError                    2
#define    BaudRateError                3
#define    DataError                    4
#define    StopError                    5
#define    ParityError                  6
#define    CheckSumError                7
#define    ComPortNotOpen               8
#define    SendThreadCreateError        9
#define    SendCmdError                 10
#define    ReadComStatusError           11
#define    ResultStrCheckError          12
#define    CmdError                     13
#define    TimeOut                      15
#define    ModuleIdError                17
#define    AdChannelError               18
#define    UnderInputRange              19
#define    ExceedInputRange             20
#define    InvalidateCounterNo          21
#define    InvalidateCounterValue       22
#define    InvalidateGateMode           23
#define    InvalidateChannelNo          24
#define    ComPortInUse                 25

EXPORTS WORD CALLBACK Get_Uart_Version(void);
/*********************Get_Uart_Version****************************/
/* The function is used to obtain the version information        */
/* Return: Version number ==> For example version 2.0.4          */
/*****************************************************************/	

EXPORTS WORD  CALLBACK Open_Com(unsigned char cPort, DWORD dwBaudrate, char cData, char cParity, char cStop);
/*********************** Open_Com ********************************/
/* This function is used to configure and open the COM port      */
/* cPort:		1~255                                            */
/* dwBaudrate:	150,300,600,1200,2400,4800,9600,19200,38400,     */
/*				57600,115200,230400,460800,921600                */
/* cData:		5,6,7,8                                          */
/* cParity:		0 ==> Non Parity	(NonParity)                  */
/*				1 ==> Odd Parity	(OddParity)                  */
/*				2 ==> Even Parity	(EvenParity)                 */
/*				3 ==> Mark Parity   (MarkParity)				 */
/*				4 ==> Space Parity  (SpaceParity)				 */
/* cStop		0 ==> 1 Stop Bit	(OneStopBit)                 */
/*				1 ==> 1.5 Stop Bit	(One5StopBit)                */
/*				2 ==> 2 Stop Bit	(TwoStopBit)                 */
/* Return       NoError : 	 	OK                               */
/*				Others : 		Error code                       */
/*****************************************************************/	
EXPORTS WORD CALLBACK Change_Baudrate(unsigned char cPort, DWORD dwBaudrate);
/*********************** Change_Baudrate***************************/
/* change the baudrate 										      */
/* cPort:		1~255                                             */
/* dwBaudrate:	150,300,600,1200,2400,4800,9600,19200,38400,      */
/*              57600,115200,230400,460800,921600                 */
/* Return: 		NoError : 		OK                                */
/*				Others : 		Error code                        */
/******************************************************************/
EXPORTS WORD CALLBACK Change_Config(unsigned char cPort, DWORD dwBaudrate, char cData, char cParity, char cStop);
/********************************* Change_Config *****************************/
/* This function only can be used to change the configuration of the COM port */
/* cPort:		1~255                                             			 */
/* dwBaudrate:	150,300,600,1200,2400,4800,9600,19200,38400,      			 */
/*              57600,115200,230400,460800,921600                 			 */
/* cData:		5,6,7,8                                                      */
/* cParity:		0 ==> Non Parity	(NonParity)                              */
/*				1 ==> Odd Parity	(OddParity)                              */
/*				2 ==> Even Parity	(EvenParity)                 			 */
/*				3 ==> Mark Parity   (MarkParity)							 */
/*				4 ==> Space Parity  (SpaceParity)							 */
/* cStop		0 ==> 1 Stop Bit	(OneStopBit)                 			 */
/*				1 ==> 1.5 Stop Bit	(One5StopBit)                			 */
/*				2 ==> 2 Stop Bit	(TwoStopBit)                 			 */
/* Return: 		NoError : 		OK                                			 */
/*				Others : 		Error code                        			 */
/*****************************************************************************/

EXPORTS int CALLBACK Change_ParityErrorCheck(int port, int mode, char cErrorChar);
/*************************** Change_ParityErrorCheck *****************************/
/*                                                                               */
/* This functions is used to change the parity error check status.               */
/* cPort:	1~255                                                                */
/* mode:	Specifies whether bytes received with parity errors are replaced     */
/*			with the character specified by the cErrorChar member. If this       */
/*			member is TRUE and the fParity member is TRUE, replacement occurs    */
/* cErrorChar:																     */
/*			Specifies the value of the character used to replace bytes received  */
/*			with a parity error.												 */
/*																				 */
/* [May,24,2005] by Sean														 */
/*********************************************************************************/

EXPORTS WORD CALLBACK Get_Com_Status(unsigned char cPort);
/********************************Get_Com_Status ***************************/
/* cPort:		1~255                                                     */
/* Return: 0 ==> COM port not in use									  */
/*         1 ==> COM port in use										  */
/**************************************************************************/
EXPORTS BOOL  CALLBACK Close_Com(unsigned char cPort);
/********************************* Change_Baudrate****************************/
/*This function closes and releases the resources of the COM port			 */
/* cPort:		1~255														 */
/*****************************************************************************/
EXPORTS WORD  CALLBACK Send_Receive_Cmd(unsigned char cPort, char szCmd[], char
        szResult[], WORD wTimeOut, WORD wChksum, WORD *wT);
/********************************Send_Receive_Cmd ****************************************/
/* cPort:		1~255																	 */
/* szCmd[]:		1024 Bytes maximum, without zero (0x0D) character.						 */
/* szResult[]:	1024 Bytes maximum, with one zero or 0x0D terminal character.			 */
/* wTimeOut:	Timeout for receiving result string. 									 */
/*				Unit: ms (1/1000 second)												 */
/* wChksum:		0   ==> add one 0x0D byte to the end of the szCmd						 */
/*				<>0 ==> add two check sum bytes and one 0x0D byte to the end of the szCmd*/
/* *wT			return a reference number for identify the performance. 				 */
/*				*wT --> 0 :good 														 */
/*****************************************************************************************/
EXPORTS WORD CALLBACK Send_Receive_UserCmd(unsigned char cPort, char szCmd[], 
			char szResult[], WORD wTimeOut, WORD wChksum, WORD *wT,char sUserString[],int iUserLen);//@35
/********************************Send_Receive_Cmd ****************************************/
/* cPort:		1~255																	 */
/* szCmd[]:		1024 Bytes maximum, without zero (0x0D) character.						 */
/* szResult[]:	1024 Bytes maximum, with one zero or 0x0D terminal character.			 */
/* wTimeOut:	Timeout for receiving result string. 									 */
/*				Unit: ms (1/1000 second)												 */
/* wChksum:		0   ==> add one 0x0D byte to the end of the szCmd						 */
/*				<>0 ==> add two check sum bytes and one 0x0D byte to the end of the szCmd*/
/* *wT			return a reference number for identify the performance. 				 */
/*				*wT --> 0 :good 														 */
/* sUserString: The user-defined terminal string										 */
/* iUserLen:    The length of theuser-defined terminal string							 */
/* Note: The length of szCmd and sUserString string cannot over 1024 bytes				 */		
/*****************************************************************************************/
EXPORTS WORD  CALLBACK Send_Cmd(unsigned char cPort, char szCmd[],WORD wChksum);
/********************** Send_Cmd *************************************/
/*This function only sends a command string to DCON series modules   */
/*********************************************************************/
EXPORTS WORD CALLBACK Receive_Cmd(unsigned char cPort, char szResult[], WORD wTimeOut, WORD wChksum, WORD *wT);
/********************* Receive_Cmd ***********************************/
/*This function only sends a command string to DCON series modules   */
/*********************************************************************/
EXPORTS WORD CALLBACK Send_Binary(unsigned char cPort, char szCmd[],int iLen);
/********************** Send_Binary***********************************/
/* Send out the command string by the fixed length                   */
/* To use protocol that not with 0x0d(CR) end.                       */
/* The function will check 0x0 or 0x20(Space) end.                   */
/* The input buffer will be cleared before sending out string.       */
/*********************************************************************/
EXPORTS WORD CALLBACK Receive_Binary(unsigned char cPort, char szResult[], WORD wTimeOut, WORD wLen, WORD *wT);
/********************** Receive_Binary *******************************/
/* This function is applied to receive the fix length response       */ 
/* To get response that not with 0x0d(CR) end.                       */
/* The function will recive wLen bytes.                              */
/*********************************************************************/
EXPORTS WORD CALLBACK ReadComn(unsigned char cPort, char szResult[], WORD wmaxLen, WORD *wT);
/**************************** ReadComn ************************************/
/* This function is applied to receive the multi-bytes response from      */ 
/* COM port                                                               */
/*	cPort:		1~255													  */
/*  szResult[]: Pointer to the buffer that receives the data read         */
/*  from COM port.                                                        */
/*  wmaxLen:    Number of bytes to be read from COM port                  */
/*  *wT:		return a reference number for identify the performance.   */
/*			                                           					  */
/*	Return: The number of bytes actually read.                            */
/**************************************************************************/
EXPORTS DWORD CALLBACK DataSizeInCom(int port);
/******************** DataSizeOutCom *********************************/
/* Get the how many data existed on the input buffer of COM port     */
/*********************************************************************/
EXPORTS DWORD CALLBACK DataSizeOutCom(int port);
/******************** DataSizeOutCom *********************************/
/* Get the how many data existed on the output buffer of COM port    */
/*********************************************************************/
EXPORTS WORD CALLBACK SetLineStatus(int port,int pin,int mode);
/*************** SetLineStatus ***************/
/* Set the Line status(DTR,RTS)              */ 
/*     pin 0:                                */
/*		   1: DTR                            */
/*		   2: RTS                            */
/*		   3: DTR+RTS                        */
/*	   mode 0:Disable                        */
/*		    1:Enable                         */
/*			2:HandShake                      */
/* return:                                   */
/*		  0,ok                               */
/*        < 0,error                          */
/*********************************************/
EXPORTS WORD CALLBACK GetLineStatus(int port,int pin);
/*************** GetLineStatus ***************/
/*                                           */
/* Get the Line status (CTS,DSR,RI,CD)       */ 
/*     pin 0: CTS							 */
/*		   1: DSR							 */
/*		   2: RI							 */
/*		   3: CD     					     */
/* return:									 */
/*		  1,ON                               */
/*        0,OFF 							 */
/*********************************************/
EXPORTS WORD CALLBACK Send_Receive_Cmd_WithChar(unsigned char cPort, char szCmd[], char szResult[], WORD wTimeOut, WORD wChksum, WORD *wT,unsigned char cEndChar);
/*********** Send_Receive_Cmd_WithChar ***************************/
/* Send command and then receive datas                           */
/* with the last character						                 */
/* It is similar to the Send_Receive_Cmd except cEndChar paramter*/
/*****************************************************************/
EXPORTS WORD CALLBACK Send_Cmd_WithChar(unsigned char cPort, char szCmd[],WORD wChksum,unsigned char cEndChar);
/******************************* Send_Cmd_WithChar *******************/
/* Send command with a character	                                 */
/* in the end of string                                              */
/* It is similar to the Send_Cmd_WithChar except cEndChar paramter   */
/*********************************************************************/
EXPORTS WORD CALLBACK Receive_Cmd_WithChar(unsigned char cPort,char szResult[], WORD wTimeOut, WORD wChksum, WORD *wT,unsigned char cEndChar);
/************************** Receive_Cmd_WithChar **********************/
/* Receive command with the last character                            */
/* It is similar to the Receive_Cmd_WithChar except cEndChar paramter */
/**********************************************************************/

#ifdef _WIN32_WCE
EXPORTS	int CALLBACK Receive_Binary_MOS(int cPort, char *szResult,int wLen);
#else
EXPORTS int CALLBACK Receive_Binary_MOS(unsigned char cPort, LPVOID szResult,WORD wLen);
#endif
//EXPORTS WORD CALLBACK Receive_Binary_MOS(unsigned char cPort, LPVOID szResult,WORD wLen);
/************* Receive_Binary_MOS ************/
/* This function is called by MiniOS7 only.  */
/* Ensure to receive the wLen length of		 */
/* the szResult.							 */
/*********************************************/

EXPORTS WORD CALLBACK Set_FlowControl(unsigned char cPort,int DCB_Member,int mode);
/**********************************  Set_FlowControl **********************************************************/
/*													
/* This function is used to set DCB settings which are related to the
/* flow control (Software and Hardware).
/* Software flow control:
/*		Use XOFF and XON characters to control the transmission and reception of data.
/* Hareware flow control:
/*		Use control lines of the serial cable to control whether sending or receiving is enabled.	

/* DCB_Member: 0 CtS (fOutxCtsFlow)
/*			   1 Rts (fRtsControl)
/*			   2 Dsr (fOutxDsrFlow)
/*			   3 Dtr (fDtrControl)
/*			   4 Tx	 (fOutx :Tx XON/XOFF flow control)
/*			   5 Rx  (fInx  :Rx XON/XOFF flow control)

/* Mode:
/*		CTS : 1: Enable CTS to monitored for output flow control, 0:Disable CTS flow control
/*		RTS : 1: Enable RTS flow control, 0:Disable RTS flow control, 2:RTS_CONTROL_HANDSHAKE, 3:RTS_CONTROL_TOGGLE
/*		DSR	: 1: Enable DSR flow control / 0:Disable DSR flow control
/*		DTR : 1: Enable DTR flow control, 0:Disable DTR flow control, 2:DTR_CONTROL_HANDSHAKE
/*		TX  : 1: Enable TX XON/XOFF, 0: Disable TX XON/XOFF
/*		RX  : 1: Enable RX XON/XOFF, 0: Disable RX XON/XOFF
/* [Nov,21,2003] by Sean
/**************************************************************************************************************/


EXPORTS WORD CALLBACK Get_FlowControl(unsigned char cPort,int DCB_Member);
/*********************************** Get_FlowControl ***********************************************/
/*													
/* This function is used to get DCB settings which are related to the
/* flow control (Software and Hardware).

/* DCB_Member: 0 CtS (fOutxCtsFlow)
/*			   1 Rts (fRtsControl)
/*			   2 Dsr (fOutxDsrFlow)
/*			   3 Dtr (fDtrControl)
/*			   4 Tx	 (fOutx :Tx XON/XOFF flow control)
/*			   5 Rx  (fInx  :Rx XON/XOFF flow control)
/* return:									 
/*		 CtS :	0:fOutxCtsFlow value is False
/*				1:fOutxCtsFlow status is True
/*		 Rts : fRtsControl value
/*				0:RTS_CONTROL_DISABLE
/*				1:RTS_CONTROL_ENABL
/*				2:RTS_CONTROL_HANDSHAKE	
/*				3:RTS_CONTROL_TOGGLE
/*		 Dsr :	0:fOutxDsrFlow value is False  
				1:fOutxDsrFlow value is True
/*		 Dtr : fDtrControl value
/*  			0:DTR_CONTROL_DISABLE, 
/*				1:DTR_CONTROL_ENABL
/*				2:DTR_CONTROL_HANDSHAKE
/*		 Tx	 :	0: fOutx value is False.
				1: fOutx value is True, XON/XOFF flow control is used during transmission
/*		 Rx  :	0: fInx value is False
/*				1: fInx value is True, XON/XOFF flow control is used during reception
/*
/* [Nov,21,2003] by sean                     
/*****************************************************************************************************/
#ifndef _WIN32_WCE  //Provided for WinCon
	EXPORTS WORD CALLBACK Set_Break(unsigned char cPort,unsigned char cAction);
#else
	EXPORTS WORD CALLBACK Set_Break(int port, int action);
#endif
/*********************************** Set_Break ***********************************************/
/*													
/* This function is used to send the Break signal for the COM port.
/* [Feb,2,2004] by sean                     
/*****************************************************************************************************/

EXPORTS WORD CALLBACK ModbusGetCRC16(char puchMsg[],unsigned char CRC[], int DataLen);
/**************** ModbusGetCRC16  ****************/
/*                                               */
/* Get the 16-bit CRC from the CRC table         */
/*                                               */
/* [Dec,12,2004] by Martin                       */
/*************************************************/

EXPORTS WORD CALLBACK Send_Receive_Binary(unsigned char cPort, char ByteCmd[],WORD in_Len,char ByteResult[], WORD out_Len, WORD wTimeOut);
/********************************Send_Receive_Binary *************************************/
/* Send binary command and receive binary data with the fixed length                     */
/*                                                                                       */
/* cPort:		 1~255																	 */
/* ByteCmd[]:	 Pointer to data to be send                       						 */
/* in_Len:       Maximum number of bytes to send                                         */
/* ByteResult[]: Buffer into which received data will be stored.                  	     */
/* out_Len:		 Maximum number of bytes to be received          						 */
/* wTimeOut:	 Timeout for receiving result string. 									 */
/*				 Unit: ms (1/1000 second)												 */
/*****************************************************************************************/

/*#ifdef _WIN32_WCE
	#endif  //__UART_H__
#endif*/
