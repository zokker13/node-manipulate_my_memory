#include "windows_extension.hpp"

void _ExReadProcessMemory(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
  int value = 0;
  SIZE_T bytesRead = 0;
  BOOL success = ReadProcessMemory(HANDLE(info[0]->IntegerValue()), (void *)info[1]->IntegerValue(), &value, (SIZE_T)info[2]->IntegerValue(), &bytesRead);

  cout << "Success: " << success << endl;
  cout << "Value: " << value << endl;
  cout << "Bytes read: " << bytesRead << endl;


  info.GetReturnValue().Set(Nan::New((int)value)->ToInteger());
}

NAN_METHOD(NanWin32OpenProcess)
{
  Callback *cb = new Callback(info[3].As<Function>());

  AsyncQueueWorker(new Win32OpenProcess(cb, info[0]->NumberValue(), info[1]->BooleanValue(), info[2]->NumberValue()));
}

NAN_METHOD(NanWin32CloseHandle)
{
  Callback *cb = new Callback(info[1].As<Function>());

  AsyncQueueWorker(new Win32CloseHandle(cb, HANDLE(int(info[0]->NumberValue()))));
}

NAN_METHOD(NanWin32ReadProcessMemory)
{
  Callback *cb = new Callback(info[3].As<Function>());

  AsyncQueueWorker(new Win32ReadProcessMemory(cb, HANDLE(int(info[0]->IntegerValue())), reinterpret_cast<void *>(info[1]->IntegerValue()), static_cast<SIZE_T>(info[2]->IntegerValue())));
}
