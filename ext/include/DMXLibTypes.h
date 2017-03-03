// DMXLibTypes.h
//
// Copyright 2001, Color Kinetics Inc.
//
// All the data types that programmers will
// need to use the exported functionality of
// the new DMX library.
//
// 02-Feb-2001	jwarwick
//				Created file.
//

#ifndef _DMXLIBTYPES_H_
#define _DMXLIBTYPES_H_


// the datatype for dates and sizes
typedef LONGLONG DMXLIB_int64;

// the value used to access opened controllers
typedef int DevID;

// the bitfield used to list the classes a device
// belongs to
typedef long DevClasses;

// the unique ID of USB devices
typedef long DevNumber;

// the maximum length of a pathname
#define DMXLIB_MAX_PATH 256

// the maximum length of a comment
#define DMXLIB_MAX_COMMENT 256


// the types of supported devices
enum DevType {SJ1, SJ2, SJ3, IPLAYER1, IPLAYER2};

// the connection points for supported devices
//enum DevPort {COM1, COM2, COM3, COM4, LPT1, LPT2, USB};
enum DevPort{COM, LPT, USB};

// the different states the driver can be in
enum DevState {IDLE, LIVEPLAY, PLAYING, RECORDING, WRITING, 
				READING, TESTING, TRIGGERING};

// the types of supported triggers
enum DevTriggerType {BINARY, ANALOG, TIME};

// the types of supported trigger actions
enum DevTriggerActionType {PLAYSHOW};

// function return values
enum DevReturn {
	SUCCESS,
	FAILURE,
	END_LIST,
	NOT_IMPLEMENTED,
	NOT_SUPPORTED,
	SER_OPEN_ERROR,
	SER_BUFF_ERROR,
	SER_COMM_ERROR,
	SER_DCB_ERROR,
	SER_ESCAPE_ERROR,
	OUT_OF_MEMORY,
	UNKNOWN_DEVICE_TYPE,
	UNKNOWN_DEVICE_PORT,
	UNKNOWN_DEVID,
	INVALID_STATE,
	WRONG_CLASS,
	DEVICE_OUT_OF_MEMORY,
	BAD_MAGIC_NUMBER,
	ACK_TIMEOUT,
	DATA_TIMEOUT,
	CHECKSUM_TIMEOUT,
	DEVICE_NAK,
	BAD_CHECKSUM,
	INVALID_FRAME,
	CREATE_FILE_ERROR,
	WRITE_ERROR,
	SEEK_ERROR,
	READ_ERROR,
	OPEN_FILE_ERROR,
	UNKNOWN_FILE,
	INVALID_VERSION,
	MISSING_COMPONENT,
	FILE_ALREADY_EXISTS
};


// the description of a supported device
struct DevEntry{
	DevType deviceType;
	wchar_t devicePort[DMXLIB_MAX_PATH];
	DevClasses deviceClasses;
	DevNumber deviceNumber;
	wchar_t deviceName[DMXLIB_MAX_PATH];
};

// information about a specific controller
struct DevInfo{
	int firmwareMajorRevision;
	int firmwareMinorRevision;
	int numberPlaybackChannels;
	bool canPlaybackCompress;
	int numberPlaybackTriggers;
	DMXLIB_int64 totalSpace;
	int maxNumberShows;
};

// description of a trigger
struct DevTrigger{
	DevTriggerType triggerType;
	long value;
};

// description of a trigger action
struct DevTriggerAction{
	DevTriggerActionType actionType;
	wchar_t fileName[DMXLIB_MAX_PATH];
	long value;
};


// description of a file on a device
struct DevFileEntry{
	wchar_t fileName[DMXLIB_MAX_PATH];
	DMXLIB_int64 fileSize;
	DMXLIB_int64 lastModified;
	bool isDirectory;
};


// header for a show stored on a device
struct DMXFileHeader{
	bool constantFrameRate;
	bool loopShow;
	DMXLIB_int64 date;
	wchar_t commentField[DMXLIB_MAX_COMMENT];
	int frameRate;
	int numberChannels;
	long totalFrames;
};

// a single frame of DMX data for a device
struct DMXFrame{
	int numberChannels;
	int startCode;
	long timeCode;
	unsigned char *dmxData;
};


#endif // _DMXLIBTYPES_H_