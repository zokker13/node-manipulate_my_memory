#include "process_vm_readv.hpp"

ProcessVmReadvTransformation::ProcessVmReadvTransformation()
{
}

ProcessVmReadvTransformation::ProcessVmReadvTransformation(NAN_METHOD_ARGS_TYPE info)
{
  this->FromInfo(info);
}

ProcessVmReadvTransformation::~ProcessVmReadvTransformation()
{
  cout << "Free all the memory" << endl;
  delete[] this->cpBuffer;
}


void ProcessVmReadvTransformation::Exec()
{
  this->bSuccess = ReadProcessMemory(this->hProcess, this->lpBaseAddress, this->cpBuffer, this->nSize, &this->uiNUmberOfBytesRead);
}

void ProcessVmReadvTransformation::FromInfo(NAN_METHOD_ARGS_TYPE info)
{
  this->hProcess = reinterpret_cast<HANDLE>(info[0]->IntegerValue());
  this->lpBaseAddress = reinterpret_cast<LPCVOID>(info[1]->IntegerValue());
  this->nSize = static_cast<SIZE_T>(info[2]->IntegerValue());
  this->cpBuffer = new char[this->nSize];
}


LinuxProcessVmReadv::LinuxProcessVmReadv(Callback* callback, NAN_METHOD_ARGS_TYPE info)
  : AsyncWorker(callback)
{
  this->data.FromInfo(info);
}

void LinuxProcessVmReadv::Execute()
{
  this->data.Exec();
}

void LinuxProcessVmReadv::HandleOKCallback()
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

NAN_METHOD(NanLinuxProcessVmReadv)
{
  Callback *cb = new Callback(info[3].As<Function>());

  AsyncQueueWorker(new LinuxProcessVmReadv(cb, info));
}

NAN_METHOD(NanLinuxProcessVmReadvSync)
{
  ProcessVmReadvTransformation trans = ProcessVmReadvTransformation(info);
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
