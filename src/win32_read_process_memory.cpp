#include "win32_read_process_memory.hpp"


RpmTransformation::RpmTransformation(NAN_METHOD_ARGS_TYPE info)
{
  this->hProcess = reinterpret_cast<HANDLE>(info[0]->IntegerValue());
  this->lpBaseAddress = reinterpret_cast<void *>(info[1]->IntegerValue());
  this->nSize = static_cast<SIZE_T>(info[2]->IntegerValue());
}



Win32ReadProcessMemory::Win32ReadProcessMemory(Callback* callback, NAN_METHOD_ARGS_TYPE info)
  : AsyncWorker(callback)
  , uiNumberOfBytesRead(0)
  , bSuccess(false)
{
  RpmTransformation transformed = RpmTransformation(info);
  this->hProcess = transformed.hProcess;
  this->lpBaseAddress = transformed.lpBaseAddress;
  this->nSize = transformed.nSize;
  this->cpBuffer = new char[this->nSize];
}

Win32ReadProcessMemory::~Win32ReadProcessMemory()
{
  delete[] cpBuffer;
}

void Win32ReadProcessMemory::Execute()
{
  bSuccess = ReadProcessMemory(hProcess, lpBaseAddress, cpBuffer, nSize, &uiNumberOfBytesRead);
}

void Win32ReadProcessMemory::HandleOKCallback()
{
  HandleScope scope;
  v8::Isolate *isolate = v8::Isolate::GetCurrent();

  Local<ArrayBuffer> buf = Local<ArrayBuffer>::New(isolate, ArrayBuffer::New(isolate, uiNumberOfBytesRead));
  Local<v8::Uint8Array> uarr = v8::Uint8Array::New(buf, 0, uiNumberOfBytesRead);
  for (int i = 0; i < 4; i++)
  {
    uarr->Set(i, v8::Integer::New(isolate, cpBuffer[i]));
  }

  Local<Value> argv[] = {
    Null()
    , New<Boolean>(bSuccess)
    , uarr
  };

  callback->Call(3, argv);
}
