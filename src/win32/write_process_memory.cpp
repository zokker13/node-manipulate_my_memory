#include "write_process_memory.hpp"

WriteProcessMemoryTransformation::WriteProcessMemoryTransformation()
{
  this->bSuccess = false;
  this->cpBuffer = nullptr;
  this->hProcess = nullptr;
  this->lpBaseAddress = nullptr;
  this->nSize = 0;
  this->uiNUmberOfBytesWritten = 0;
  this->errorMessage = "";
}

WriteProcessMemoryTransformation::WriteProcessMemoryTransformation(NAN_METHOD_ARGS_TYPE info)
{
  this->FromInfo(info);
}

void WriteProcessMemoryTransformation::Exec()
{
  this->bSuccess = WriteProcessMemory(this->hProcess, this->lpBaseAddress, this->cpBuffer, this->nSize, &this->uiNUmberOfBytesWritten);
  
  if (!bSuccess)
  {
    LPCSTR errMsg = GetLastErrorDescription(TEXT("WriteProcessMemory"));
    throw errMsg;
  }
}

void WriteProcessMemoryTransformation::FromInfo(NAN_METHOD_ARGS_TYPE info)
{
  this->hProcess = reinterpret_cast<HANDLE>(info[0]->IntegerValue());
  this->lpBaseAddress = reinterpret_cast<LPVOID>(info[1]->IntegerValue());
  this->nSize = static_cast<SIZE_T>(info[2]->IntegerValue());

  this->cpBuffer = node::Buffer::Data(info[3]->ToObject());
}


Win32WriteProcessMemory::Win32WriteProcessMemory(Callback* callback, NAN_METHOD_ARGS_TYPE info)
  : AsyncWorker(callback)
{
  this->data.FromInfo(info);
}

void Win32WriteProcessMemory::Execute()
{
  try 
  {
    this->data.Exec();
  }
  catch (LPCSTR err) 
  {
    this->SetErrorMessage(err);
  }
}

void Win32WriteProcessMemory::HandleOKCallback()
{
  Local<Number> tempNumber = New<Uint32>(static_cast<unsigned int>(this->data.uiNUmberOfBytesWritten));


  Local<Value> argv[] = {
    Null()
    , tempNumber
  };

  callback->Call(2, argv);
}

NAN_METHOD(NanWin32WriteProcessMemory)
{
  Callback *cb = new Callback(info[4].As<Function>());

  AsyncQueueWorker(new Win32WriteProcessMemory(cb, info));
}

NAN_METHOD(NanWin32WriteProcessMemorySync)
{
  WriteProcessMemoryTransformation trans = WriteProcessMemoryTransformation(info);
  trans.Exec();

  Local<Number> tempNumber = New<Uint32>(static_cast<unsigned int>(trans.uiNUmberOfBytesWritten));
  info.GetReturnValue().Set(tempNumber);
}
