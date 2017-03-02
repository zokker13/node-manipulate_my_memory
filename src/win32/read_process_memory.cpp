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

void ReadProcessMemoryTransformation::Exec()
{
  this->bSuccess = ReadProcessMemory(this->hProcess, this->lpBaseAddress, this->cpBuffer, this->nSize, &this->uiNUmberOfBytesRead);

  if (bSuccess == FALSE)
  {
    delete[] this->cpBuffer;
    cout << "Feels Bad Man" << endl;
  }
    
}

void ReadProcessMemoryTransformation::FromInfo(NAN_METHOD_ARGS_TYPE info)
{
  this->hProcess = reinterpret_cast<HANDLE>(info[0]->IntegerValue());
  this->lpBaseAddress = reinterpret_cast<LPCVOID>(info[1]->IntegerValue());
  this->nSize = static_cast<SIZE_T>(info[2]->IntegerValue());

  // We do allocate this buffer but never delete it. 
  // That is because the node GC takes care of it after we pass it to the Nan::NewBuffer
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
  Nan::MaybeLocal<Object> myBuf = Nan::NewBuffer(this->data.cpBuffer, this->data.uiNUmberOfBytesRead);

  Local<Value> argv[] = {
    Null()
    , myBuf.ToLocalChecked()
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

  Nan::MaybeLocal<Object> myBuf = Nan::NewBuffer(trans.cpBuffer, trans.uiNUmberOfBytesRead);

  info.GetReturnValue().Set(myBuf.ToLocalChecked());
}
