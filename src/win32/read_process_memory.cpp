#include "read_process_memory.hpp"

ReadProcessMemoryTransformation::ReadProcessMemoryTransformation()
{
  this->bSuccess = false;
  this->cpBuffer = nullptr;
  this->hProcess = nullptr;
  this->lpBaseAddress = nullptr;
  this->nSize = 0;
  this->uiNUmberOfBytesRead = 0;
}

ReadProcessMemoryTransformation::ReadProcessMemoryTransformation(NAN_METHOD_ARGS_TYPE info)
{
  this->FromInfo(info);
}

ReadProcessMemoryTransformation::~ReadProcessMemoryTransformation()
{
  cout << "Free all the memory" << endl;
  delete[] this->cpBuffer;
}


void ReadProcessMemoryTransformation::Exec()
{
  this->bSuccess = ReadProcessMemory(this->hProcess, this->lpBaseAddress, this->cpBuffer, this->nSize, &this->uiNUmberOfBytesRead);
}

void ReadProcessMemoryTransformation::FromInfo(NAN_METHOD_ARGS_TYPE info)
{
  this->hProcess = reinterpret_cast<HANDLE>(info[0]->IntegerValue());
  this->lpBaseAddress = reinterpret_cast<LPCVOID>(info[1]->IntegerValue());
  this->nSize = static_cast<SIZE_T>(info[2]->IntegerValue());
  this->cpBuffer = new char[this->nSize];
}


Win32ReadProcessMemory::Win32ReadProcessMemory(Callback* callback, NAN_METHOD_ARGS_TYPE info)
  : AsyncWorker(callback)
{
  this->data.FromInfo(info);
}

void Win32ReadProcessMemory::Execute()
{
  this->data.Exec();
}

void Win32ReadProcessMemory::HandleOKCallback()
{
  HandleScope scope;
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
  };

  callback->Call(3, argv);
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
