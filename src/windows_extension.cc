#include "windows_extension.h"

void ExReadProcessMemory(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
  if (info.Length() != 3)
  {
    Nan::ThrowTypeError("Wrong Number of Arguments");
    return;
  }

  if (!info[0]->IsNumber())
  {
    Nan::ThrowTypeError("Type of argument \"hProcess\" must be positive number");
    return;
  }

  if (!info[1]->IsNumber())
  {
    Nan::ThrowTypeError("Type of argument \"lpBaseAddress\" must be positive number");
    return;
  }

  if (!info[2]->IsNumber())
  {
    Nan::ThrowTypeError("Type of argument \"nSize\" must be positive number");
    return;
  }

  int value = 0;
  SIZE_T bytesRead = 0;
  BOOL success = ReadProcessMemory(HANDLE(info[0]->IntegerValue()), (void *)info[1]->IntegerValue(), &value, (SIZE_T)info[2]->IntegerValue(), &bytesRead);

  cout << "Success: " << success << endl;
  cout << "Value: " << value << endl;
  cout << "Bytes read: " << bytesRead << endl;


  info.GetReturnValue().Set(Nan::New((int)value)->ToInteger());
}

CExOpenProcess::CExOpenProcess(Callback* callback, DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId) : AsyncWorker(callback), dwDesiredAccess(dwDesiredAccess), bInheritHandle(bInheritHandle), dwProcessId(dwProcessId), hOpenProcess(nullptr)
{

}

void CExOpenProcess::Execute()
{
  hOpenProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
}

void CExOpenProcess::HandleOKCallback()
{
  HandleScope scope;

  Local<Value> argv[] = {
    Null()
    , New<Number>(int(hOpenProcess))
  };

  callback->Call(2, argv);
}

CExCloseHandle::CExCloseHandle(Callback* callback, HANDLE hProcess) : AsyncWorker(callback), hProcess(hProcess), bSuccess(false)
{

}

void CExCloseHandle::Execute()
{
  bSuccess = CloseHandle(hProcess);
}

void CExCloseHandle::HandleOKCallback()
{
  HandleScope scope;

  Local<Value> argv[] = {
    Null()
    , New<Boolean>(bSuccess)
  };

  callback->Call(2, argv);
}

NAN_METHOD(ExOpenProcess)
{
  Callback *cb = new Callback(info[3].As<Function>());

  AsyncQueueWorker(new CExOpenProcess(cb, info[0]->NumberValue(), info[1]->BooleanValue(), info[2]->NumberValue()));
}

NAN_METHOD(ExCloseHandle)
{
  Callback *cb = new Callback(info[1].As<Function>());

  AsyncQueueWorker(new CExCloseHandle(cb, HANDLE(int(info[0]->NumberValue()))));
}
