#if !defined( WATCHDOGAPI_H__F20F2060_41C9_11d2_B04F_00A0248D3C5A__INCLUDE_ )
#define WATCHDOGAPI_H__F20F2060_41C9_11d2_B04F_00A0248D3C5A__INCLUDE_

#if !defined( DLLEXPORT )
	#define DLLEXPORT __declspec( dllexport ) 
#endif

#ifdef __cplusplus
	extern "C" {
#endif

/* NEW API FUNCTIONS */

DLLEXPORT HANDLE WINAPI BBWatchdog_Startup( UINT nID );
DLLEXPORT BOOL WINAPI BBWatchdog_Shutdown( HANDLE hDevice );
DLLEXPORT BOOL WINAPI BBWatchdog_Reset( HANDLE hDev, BYTE byDataValue );
DLLEXPORT BOOL WINAPI BBWatchdog_ReadRegister( HANDLE hDev, BYTE* pDataValue );

/* OLD API FUNCTIONS */

DLLEXPORT HANDLE WINAPI BBPCWDT_Startup( void );
DLLEXPORT BOOL WINAPI BBPCWDT_Shutdown( HANDLE hDev );
DLLEXPORT BOOL WINAPI BBPCWDT_Reset( HANDLE hDev, UCHAR DataValue );
DLLEXPORT BOOL WINAPI BBPCWDT_ReadRegister(HANDLE hDev, UCHAR* DataValue);

#ifdef __cplusplus
	}
#endif

#endif
