#ifndef WIN32_OPEN_PROCESS_HPP_
#define WIN32_OPEN_PROCESS_HPP_

#include "win32_shared.hpp"

class Win32OpenProcess : public AsyncWorker
{
public:
  Win32OpenProcess(Callback *callback, DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
  void Execute();
  void HandleOKCallback();

private:
  DWORD dwDesiredAccess;
  BOOL bInheritHandle;
  DWORD dwProcessId;
  HANDLE hOpenProcess;
};


#endif