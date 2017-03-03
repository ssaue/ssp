// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#ifndef COMPILER_H
#define COMPILER_H

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN	// Exclude rarely-used items from Windows headers.
#endif

/*
** Windows version control. Modify as follows:
*/



/*
** DO NOT MODIFY BELOW THIS LINE:
*/
 
#ifndef WINVER		          // Permit use of features specific to Windows 95 and Windows NT 4.0 or later.
#define WINVER 0x0400	      // Change this to the appropriate value to target 
#endif                      // Windows 98 and Windows 2000 or later.

#ifndef _WIN32_WINNT	      
#define _WIN32_WINNT 0x0500	// Permit use of features specific to Windows 2000 or later.
#endif

#ifndef _WIN32_WINDOWS	      
#define _WIN32_WINDOWS 0x0410 // Permit use of features specific to Windows 98 or later.
#endif

#ifndef _WIN32_IE		        // Permit use of features specific to Internet Explorer 4.0 or later.
#define _WIN32_IE 0x0400    // Change this to the appropriate value to target 
#endif			                // Internet Explorer 5.0 or later.

#endif
