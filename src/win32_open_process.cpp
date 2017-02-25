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
  this->dwDesiredAccess = info[0]->IntegerValue();
  this->bInheritHandle = info[1]->BooleanValue();
  this->dwProcessId = info[2]->IntegerValue();
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
    , New<Number>(reinterpret_cast<int>(this->data.hOpenProcess))
  };

  callback->Call(2, argv);
}