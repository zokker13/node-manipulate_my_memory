#include "win32_close_handle.hpp"

Win32CloseHandle::Win32CloseHandle(Callback* callback, HANDLE hProcess) 
  : AsyncWorker(callback)
  , hProcess(hProcess)
  , bSuccess(false)
{

}

void Win32CloseHandle::Execute()
{
  bSuccess = CloseHandle(hProcess);
}

void Win32CloseHandle::HandleOKCallback()
{
  HandleScope scope;

  Local<Value> argv[] = {
    Null()
    , New<Boolean>(bSuccess)
  };

  callback->Call(2, argv);
}