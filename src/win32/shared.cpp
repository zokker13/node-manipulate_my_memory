#include "shared.hpp"

LPCTSTR GetLastErrorDescription(LPTSTR lpszFunction) 
{
  LPVOID lpMsgBuf;
  LPVOID lpDisplayBuf;
  DWORD dw = GetLastError();

  FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER |
    FORMAT_MESSAGE_FROM_SYSTEM |
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    dw,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR) &lpMsgBuf,
    0,
    NULL
  );

  int len = lstrlen((LPCTSTR)lpMsgBuf);
  char *nicerDimension = new char[len - 1]; // Drop \r\n
  memcpy(nicerDimension, lpMsgBuf, len);
  nicerDimension[len - 2] = 0;


  lpDisplayBuf = (LPVOID)LocalAlloc(
    LMEM_ZEROINIT,
    (strlen(nicerDimension) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)
  );
  StringCchPrintf((LPTSTR)lpDisplayBuf,
    LocalSize(lpDisplayBuf) / sizeof(TCHAR),
    TEXT("[%s] failed with \"%s\" (%d)"),
    lpszFunction,
    nicerDimension,
    dw
  );

  // Win10 - probably really bad
  HeapFree(GetProcessHeap(), 0, lpMsgBuf);
  HeapFree(GetProcessHeap(), 0, lpDisplayBuf);

  return (LPCTSTR)lpDisplayBuf;
}