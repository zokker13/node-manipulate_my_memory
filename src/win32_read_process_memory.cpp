#include "win32_read_process_memory.hpp"

Win32ReadProcessMemory::Win32ReadProcessMemory(Callback* callback, HANDLE hProcess, LPCVOID lpBaseAddress, SIZE_T nSize)
  : AsyncWorker(callback)
  , hProcess(hProcess)
  , lpBaseAddress(lpBaseAddress)
  , nSize(nSize)
  , cpBuffer(new char[nSize])
  , uiNumberOfBytesRead(0)
  , bSuccess(false)
{

}

Win32ReadProcessMemory::~Win32ReadProcessMemory()
{
  cout << "DELETED THE DATA" << endl;
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