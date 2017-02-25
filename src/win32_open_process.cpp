#include "win32_open_process.hpp"

OpTransformation::OpTransformation()
{
  this->dwDesiredAccess = 0;
  this->bInheritHandle = false;
  this->dwProcessId = 0;
  this->hOpenProcess = nullptr;
}

OpTransformation::OpTransformation(NAN_METHOD_ARGS_TYPE info)
{
  this->FromInfo(info);
}

void OpTransformation::Exec()
{
  this->hOpenProcess = OpenProcess(this->dwDesiredAccess, this->bInheritHandle, this->dwProcessId);
}

void OpTransformation::FromInfo(NAN_METHOD_ARGS_TYPE info)
{
  this->dwDesiredAccess = info[0]->Uint32Value();
  this->bInheritHandle = info[1]->BooleanValue();
  this->dwProcessId = info[2]->Uint32Value();
  this->hOpenProcess = nullptr;
}

Win32OpenProcess::Win32OpenProcess(Callback* callback, NAN_METHOD_ARGS_TYPE info)
  : AsyncWorker(callback)
{
  this->data.FromInfo(info);
}

void Win32OpenProcess::Execute()
{
  this->data.Exec();
}

void Win32OpenProcess::HandleOKCallback()
{
  HandleScope scope;

  Local<Value> argv[] = {
    Null()
    , New<Uint32>(reinterpret_cast<unsigned int>(this->data.hOpenProcess))
  };

  callback->Call(2, argv);
}

NAN_METHOD(NanWin32OpenProcess)
{
  Callback *cb = new Callback(info[3].As<Function>());

  AsyncQueueWorker(new Win32OpenProcess(cb, info));
}

NAN_METHOD(NanWin32OpenProcessSync)
{
  OpTransformation trans = OpTransformation(info);
  trans.Exec();


  Local<Number> tempNumber = New<Uint32>(reinterpret_cast<unsigned int>(trans.hOpenProcess));
  info.GetReturnValue().Set(tempNumber);
}
