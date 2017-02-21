#include <iostream>

#include <nan.h>

#include "Windows.h"

using namespace std;

#ifndef WINDOWS_EXTENSION_H_
#define WINDOWS_EXTENSION_H_

void ExReadProcessMemory(const Nan::FunctionCallbackInfo<v8::Value>& info);
void ExOpenProcess(const Nan::FunctionCallbackInfo<v8::Value>& info);
void ExCloseHandle(const Nan::FunctionCallbackInfo<v8::Value>& info);

#endif