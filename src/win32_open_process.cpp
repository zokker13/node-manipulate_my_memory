#include "win32_open_process.hpp"

Win32OpenProcess::Win32OpenProcess(Callback* callback, DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId) 
  : AsyncWorker(callback)
  , dwDesiredAccess(dwDesiredAccess)
  , bInheritHandle(bInheritHandle)
  , dwProcessId(dwProcessId)
  , hOpenProcess(nullptr)
{

}

void Win32OpenProcess::Execute()
{
  hOpenProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
}

void Win32OpenProcess::HandleOKCallback()
{
  HandleScope scope;

  Local<Value> argv[] = {
    Null()
    , New<Number>(int(hOpenProcess))
  };

  callback->Call(2, argv);
}