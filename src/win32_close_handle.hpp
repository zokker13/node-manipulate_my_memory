#ifndef WIN32_CLOSE_HANDLE_HPP_
#define WIN32_CLOSE_HANDLE_HPP_

#include "win32_shared.hpp"

class Win32CloseHandle : public AsyncWorker
{
public:
  Win32CloseHandle(Callback *callback, HANDLE hProcess);
  void Execute();
  void HandleOKCallback();

private:
  BOOL bSuccess;
  HANDLE hProcess;
};

#endif