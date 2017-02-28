#include "close_handle.hpp"

CloseHandleTransformation::CloseHandleTransformation()
{
  this->bSuccess = false;
  this->hOpenProcess = nullptr;
}

CloseHandleTransformation::CloseHandleTransformation(NAN_METHOD_ARGS_TYPE info)
{
  this->FromInfo(info);
}

void CloseHandleTransformation::Exec()
{
  this->bSuccess = CloseHandle(this->hOpenProcess);
}

void CloseHandleTransformation::FromInfo(NAN_METHOD_ARGS_TYPE info)
{
  this->hOpenProcess = reinterpret_cast<HANDLE>(info[0]->Uint32Value());
}

Win32CloseHandle::Win32CloseHandle(Callback* callback, NAN_METHOD_ARGS_TYPE info)
  : AsyncWorker(callback)
{
  this->data.FromInfo(info);
}

void Win32CloseHandle::Execute()
{
  this->data.Exec();
}

void Win32CloseHandle::HandleOKCallback()
{
  HandleScope scope;

  Local<Value> argv[] = {
    Null()
    , New<Boolean>(this->data.bSuccess)
  };

  callback->Call(2, argv);
}

NAN_METHOD(NanWin32CloseHandle)
{
  Callback *cb = new Callback(info[1].As<Function>());

  AsyncQueueWorker(new Win32CloseHandle(cb, info));
}

NAN_METHOD(NanWin32CloseHandleSync)
{
  CloseHandleTransformation trans = CloseHandleTransformation(info);
  trans.Exec();
  info.GetReturnValue().Set(static_cast<bool>(trans.bSuccess));
}
