#include "windows_extension.hpp"

NAN_METHOD(NanWin32OpenProcess)
{
  Callback *cb = new Callback(info[3].As<Function>());

  AsyncQueueWorker(new Win32OpenProcess(cb, info));
}

NAN_METHOD(NanWin32OpenProcessSync)
{
  OpTransformation trans = OpTransformation(info);
  trans.Exec();
  info.GetReturnValue().Set(reinterpret_cast<int>(trans.hOpenProcess));
}

NAN_METHOD(NanWin32CloseHandle)
{
  Callback *cb = new Callback(info[1].As<Function>());

  AsyncQueueWorker(new Win32CloseHandle(cb, HANDLE(int(info[0]->NumberValue()))));
}

NAN_METHOD(NanWin32ReadProcessMemory)
{
  Callback *cb = new Callback(info[3].As<Function>());

  AsyncQueueWorker(new Win32ReadProcessMemory(cb, info));
}

NAN_METHOD(NanWin32ReadProcessMemorySync)
{
  
}
