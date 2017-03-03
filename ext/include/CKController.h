// CKController.h
//
// Copyright 2001, Color Kinetics Inc.
//
// Functions exported from CKController.dll
//
// 02-Feb-2001	jwarwick
//				Created file.
//
// 11-Apr-2001	jwarwick
//				Moved to CKController.h from DMXLib2.h	
//

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DMXLIB2_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DMXLIB2_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CKCONTROLLER_EXPORTS
#define CKCONTROLLER_API __declspec(dllexport)
#else
#define CKCONTROLLER_API __declspec(dllimport)
#endif // CKCONTROLLER_EXPORTS

#ifndef _CKCONTROLLER_H_
#define _CKCONTROLLER_H_

// include all of the type information
#include "DMXLibTypes.h"

///////////////////////////////////////////////////////
// Exported utility functions
///////////////////////////////////////////////////////

// get the major revision portion of the library version
CKCONTROLLER_API int getLibraryMajorRevision();
// get the minor revision portion of the library version
CKCONTROLLER_API int getLibraryMinorRevision();
// get the build of the library
CKCONTROLLER_API int getLibraryBuildRevision();

// returns true if cls supports the live play functions
CKCONTROLLER_API bool supportsLivePlay(DevClasses cls);
// returns true if cls supports the playback config functions
CKCONTROLLER_API bool supportsPlayBackConfig(DevClasses cls);
// returns true if cls supports the playback control functions
CKCONTROLLER_API bool supportsPlayBackControl(DevClasses cls);
// returns true if cls supports the record functions
CKCONTROLLER_API bool supportsRecord(DevClasses cls);

///////////////////////////////////////////////////////
// Manager layer functions
///////////////////////////////////////////////////////

// get the first device and intialize the iterator handle
CKCONTROLLER_API DevReturn getFirstDevice(DevEntry *de);
// get the next device in the list
CKCONTROLLER_API DevReturn getNextDevice(DevEntry *de);

// open a device
CKCONTROLLER_API DevReturn openDevice(DevID* id, const DevEntry *de);
// close a device
CKCONTROLLER_API DevReturn closeDevice(DevID);


// get the device driver state
CKCONTROLLER_API DevReturn getDeviceState(DevID id, DevState *ds);
// get the device info
CKCONTROLLER_API DevReturn getDeviceInfo(DevID id, DevInfo *info);

///////////////////////////////////////////////////////
// Live Play functions
///////////////////////////////////////////////////////

// start a liveplay session
CKCONTROLLER_API DevReturn startLivePlay(DevID);

// send a frame to be played live
CKCONTROLLER_API DevReturn sendFrame(DevID, const DMXFrame *);

// terminate a liveplay session
CKCONTROLLER_API DevReturn stopLivePlay(DevID);

///////////////////////////////////////////////////////
// Playback config functions
///////////////////////////////////////////////////////

// create a new playback file
CKCONTROLLER_API DevReturn createFile(DevID id, const wchar_t *filename,
								 DMXFileHeader *header, bool compress);
// write frames to a file
CKCONTROLLER_API DevReturn writeFrames(DevID id, int numFrames, 
								  const DMXFrame *frames);

// open a file for reading
CKCONTROLLER_API DevReturn openFile(DevID id, const wchar_t *filename,
							   DMXFileHeader *header);
// read a frame from the file
CKCONTROLLER_API DevReturn readFrame(DevID id, DMXFrame *frame);

// close the device (reading or writing)
CKCONTROLLER_API DevReturn closeFile(DevID id);

// delete a file
CKCONTROLLER_API DevReturn deleteFile(DevID id, const wchar_t *filename,
								 void (*fun)(long, long));

// get the free space remaining on the device
CKCONTROLLER_API DevReturn getFreeSpace(DevID id, DMXLIB_int64 *space);

// creates a new trigger file
CKCONTROLLER_API DevReturn createTriggers(DevID id);

// set a trigger
CKCONTROLLER_API DevReturn setTrigger(DevID id, const DevTrigger *trigger,
								 const DevTriggerAction *action);

// close a trigger file
CKCONTROLLER_API DevReturn closeTriggers(DevID id);

// get the first trigger and mapping from the device
CKCONTROLLER_API DevReturn getFirstTrigger(DevID id, DevTrigger *trigger,
									  DevTriggerAction *action);
// get the next trigger
CKCONTROLLER_API DevReturn getNextTrigger(DevID id, DevTrigger *trigger,
									 DevTriggerAction *action);

// get the first file on the device
CKCONTROLLER_API DevReturn getFirstFile(DevID id, const wchar_t *path, 
									DevFileEntry *entry);
// get the next file
CKCONTROLLER_API DevReturn getNextFile(DevID id, DevFileEntry *entry);

// create a test file that does not get written to the device
CKCONTROLLER_API DevReturn createTestFile(DevID id,
									 const wchar_t *filename, 
									 const DMXFileHeader *hdr, 
									 bool compress);
// write a frame to the test file
CKCONTROLLER_API DevReturn writeTestFrames(DevID id,
									  int numberFrames, 
									  const DMXFrame *df);
// close the test file, return if it will fit on the device
// and its total size
CKCONTROLLER_API DevReturn closeTestFile(DevID id,
									bool *willFit, 
									DMXLIB_int64 *totalSize);

///////////////////////////////////////////////////////
// Playback control functions
///////////////////////////////////////////////////////

// start playing stored show
CKCONTROLLER_API DevReturn startPlayback(DevID id, const wchar_t *filename);
// stop playing a stored show
CKCONTROLLER_API DevReturn stopPlayback(DevID id);

///////////////////////////////////////////////////////
// Record functions
///////////////////////////////////////////////////////

// none in version 3.0

#endif // _CKCONTROLLER_H_