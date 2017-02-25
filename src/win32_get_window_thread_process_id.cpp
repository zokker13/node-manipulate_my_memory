#include "win32_get_window_thread_process_id.hpp"

GetWindowThreadProcessIdTransformation::GetWindowThreadProcessIdTransformation()
{
  this->hWnd = nullptr;
  this->dwThreadId = 0;
  this->dwProcessId = 0;
}

GetWindowThreadProcessIdTransformation::GetWindowThreadProcessIdTransformation(NAN_METHOD_ARGS_TYPE info)
{
  this->FromInfo(info);
}

void GetWindowThreadProcessIdTransformation::Exec()
{
  this->dwThreadId = GetWindowThreadProcessId(this->hWnd, &this->dwProcessId);
}

void GetWindowThreadProcessIdTransformation::FromInfo(NAN_METHOD_ARGS_TYPE info)
{
  this->hWnd = reinterpret_cast<HWND>(info[0]->IntegerValue());
}

Win32GetWindowThreadProcessId::Win32GetWindowThreadProcessId(Callback* callback, NAN_METHOD_ARGS_TYPE info)
  : AsyncWorker(callback)
{
  this->data.FromInfo(info);
}

void Win32GetWindowThreadProcessId::Execute()
{
  this->data.Exec();
}

void Win32GetWindowThreadProcessId::HandleOKCallback()
{
  HandleScope scope;

  Isolate *isolate = Isolate::GetCurrent();
  Local<Object> obj = Object::New(isolate);
  Local<Number> threadId = New<Number>(static_cast<unsigned __int64>(this->data.dwThreadId));
  Local<Number> processId = New<Number>(static_cast<unsigned __int64>(this->data.dwProcessId));
  obj->Set(v8::String::NewFromUtf8(isolate, "threadId"), threadId);
  obj->Set(v8::String::NewFromUtf8(isolate, "processId"), processId);

  Local<Value> argv[] = {
    Null()
    , obj
  };

  callback->Call(2, argv);
}

NAN_METHOD(NanWin32GetWindowThreadProcessId)
{
  Callback *cb = new Callback(info[1].As<Function>());

  AsyncQueueWorker(new Win32GetWindowThreadProcessId(cb, info));
}

NAN_METHOD(NanWin32GetWindowThreadProcessIdSync)
{
  GetWindowThreadProcessIdTransformation trans = GetWindowThreadProcessIdTransformation(info);
  trans.Exec();

  Isolate *isolate = Isolate::GetCurrent();
  Local<Object> obj = Object::New(isolate);
  Local<Number> threadId = New<Number>(static_cast<unsigned __int64>(trans.dwThreadId));
  Local<Number> processId = New<Number>(static_cast<unsigned __int64>(trans.dwProcessId));
  obj->Set(v8::String::NewFromUtf8(isolate, "threadId"), threadId);
  obj->Set(v8::String::NewFromUtf8(isolate, "processId"), processId);

  info.GetReturnValue().Set(obj);
}