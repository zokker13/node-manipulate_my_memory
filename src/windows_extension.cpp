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

NAN_METHOD(NanWin32FindWindow)
{
  Callback *cb = new Callback(info[2].As<Function>());

  AsyncQueueWorker(new Win32FindWindow(cb, info));
}

NAN_METHOD(NanWin32FindWindowSync)
{
  FindWindowTransformation trans = FindWindowTransformation(info);
  trans.Exec();
  info.GetReturnValue().Set(reinterpret_cast<int>(trans.handle));
}

NAN_METHOD(NanWin32ReadProcessMemory)
{
  Callback *cb = new Callback(info[3].As<Function>());

  AsyncQueueWorker(new Win32ReadProcessMemory(cb, info));
}

NAN_METHOD(NanWin32ReadProcessMemorySync)
{
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