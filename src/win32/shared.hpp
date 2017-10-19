#ifndef SHARED_HPP_
#define SHARED_HPP_

#include <iostream>

#include <nan.h>

#include <Windows.h>
#include <strsafe.h>  // FormatMessage helpers
#include <tlhelp32.h> // Snaphots etc
#include <tchar.h>

#define STR(s) Nan::New<v8::String>(s).ToLocalChecked()

using namespace std;

using v8::ArrayBuffer;
using v8::Function;
using v8::Local;
using v8::Number;
using v8::Value;
using v8::Boolean;
using v8::Isolate;
using v8::Object;
using v8::Uint32;
using v8::Array;
using Nan::NAN_METHOD_ARGS_TYPE;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::New;
using Nan::Null;
using Nan::To;

LPCTSTR GetLastErrorDescription(LPTSTR lpszFunction);
void PrintError(TCHAR* msg);

#endif
