#include <iostream>

#include <nan.h>

#include "Windows.h"

using namespace std;

using v8::Function;
using v8::Local;
using v8::Number;
using v8::Value;
using v8::Boolean;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::New;
using Nan::Null;
using Nan::To;

#ifndef WINDOWS_EXTENSION_H_
#define WINDOWS_EXTENSION_H_

void ExReadProcessMemory(const Nan::FunctionCallbackInfo<v8::Value>& info);
void ExCloseHandle(const Nan::FunctionCallbackInfo<v8::Value>& info);

NAN_METHOD(ExOpenProcess);
NAN_METHOD(ExCloseHandle);

class CExOpenProcess : public AsyncWorker
{
public:
  CExOpenProcess(Callback *callback, DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);

  void Execute();

  void HandleOKCallback();

private:
  DWORD dwDesiredAccess;
  BOOL bInheritHandle;
  DWORD dwProcessId;
  HANDLE hOpenProcess;
};

class CExCloseHandle : public AsyncWorker
{
public:
  CExCloseHandle(Callback *callback, HANDLE hProcess);

  void Execute();

  void HandleOKCallback();

private:
  BOOL bSuccess;
  HANDLE hProcess;
};

#endif