#ifndef WIN32_OPEN_PROCESS_HPP_
#define WIN32_OPEN_PROCESS_HPP_

#include "win32_shared.hpp"

struct OpTransformation
{
  OpTransformation();
  OpTransformation(NAN_METHOD_ARGS_TYPE);
  void FromInfo(NAN_METHOD_ARGS_TYPE);
  void Exec();
  DWORD dwDesiredAccess;
  BOOL bInheritHandle;
  DWORD dwProcessId;
  HANDLE hOpenProcess;
};

class Win32OpenProcess : public AsyncWorker
{
public:
  Win32OpenProcess(Callback *callback, NAN_METHOD_ARGS_TYPE info);
  void Execute();
  void HandleOKCallback();

private:
  OpTransformation data;
};


#endif