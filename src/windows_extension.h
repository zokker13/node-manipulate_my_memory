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

void _ExReadProcessMemory(const Nan::FunctionCallbackInfo<v8::Value>& info);

NAN_METHOD(ExOpenProcess);
NAN_METHOD(ExCloseHandle);
NAN_METHOD(ExReadProcessMemory);

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

class CExReadProcessMemory : public AsyncWorker
{
public:
  CExReadProcessMemory(Callback* callback, HANDLE hProcess, LPCVOID lpBaseAddress, SIZE_T nSize);
  ~CExReadProcessMemory();
  void Execute();
  void HandleOKCallback();

private:
  HANDLE hProcess;
  LPCVOID lpBaseAddress;
  char *cpBuffer;
  SIZE_T nSize;
  unsigned __int64 uiNumberOfBytesRead;
  BOOL bSuccess;
  
};

#endif