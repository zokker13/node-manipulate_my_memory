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

  AsyncQueueWorker(new Win32CloseHandle(cb, info));
}

NAN_METHOD(NanWin32CloseHandleSync)
{
  CloseHandleTransformation trans = CloseHandleTransformation(info);
  trans.Exec();
  info.GetReturnValue().Set(static_cast<bool>(trans.bSuccess));
}

NAN_METHOD(NanWin32ReadProcessMemory)
{
  Callback *cb = new Callback(info[3].As<Function>());

  AsyncQueueWorker(new Win32ReadProcessMemory(cb, info));
}

NAN_METHOD(NanWin32ReadProcessMemorySync)
{
  /*HandleScope scope;
  v8::Isolate *isolate = v8::Isolate::GetCurrent();

  Local<ArrayBuffer> buf = Local<ArrayBuffer>::New(isolate, ArrayBuffer::New(isolate, this->data.uiNUmberOfBytesRead));
  Local<v8::Uint8Array> uarr = v8::Uint8Array::New(buf, 0, this->data.uiNUmberOfBytesRead);
  for (int i = 0; i < 4; i++)
  {
    uarr->Set(i, v8::Integer::New(isolate, this->data.cpBuffer[i]));
  }

  Local<Value> argv[] = {
    Null()
    , New<Boolean>(this->data.bSuccess)
    , uarr
  };*/

  ReadProcessMemoryTransformation trans = ReadProcessMemoryTransformation(info);
  trans.Exec();

  HandleScope scope;
  v8::Isolate *isolate = v8::Isolate::GetCurrent();

  Local<ArrayBuffer> buf = Local<ArrayBuffer>::New(isolate, ArrayBuffer::New(isolate, trans.uiNUmberOfBytesRead));
  Local<v8::Uint8Array> uarr = v8::Uint8Array::New(buf, 0, trans.uiNUmberOfBytesRead);
  for (int i = 0; i < 4; i++)
  {
    uarr->Set(i, v8::Integer::New(isolate, trans.cpBuffer[i]));
  }

  
  info.GetReturnValue().Set(uarr);
}
