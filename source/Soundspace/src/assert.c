//
// assert.c
//
// Provides basic assert services for C and C++ source modules.
//

#include <windows.h>
#include "assert.h"

#ifdef _DEBUG
wchar_t szText [256];

void AssertFailedLine(LPCSTR lpszFilename, int nLine)
{
    wsprintf (szText, L"File: %s, Line: %d", lpszFilename, nLine);
    MessageBox (NULL, szText, L"ASSERT", MB_APPLMODAL | MB_ICONERROR);
}
#endif // DEBUG