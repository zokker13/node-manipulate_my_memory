#include "windows_extension.h"

void ExOpenProcess(const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (info.Length() != 3)
  {
    Nan::ThrowTypeError("Wrong Number of Arguments");
    return;
  }

  if (!info[0]->IsNumber())
  {
    Nan::ThrowTypeError("Type of argument \"dwDesiredAccess\" must be positive number");
    return;
  }

  if (!info[1]->IsBoolean())
  {
    Nan::ThrowTypeError("Type of argument \"bInheritHandle\" must be boolean");
    return;
  }

  if (!info[2]->IsNumber())
  {
    Nan::ThrowTypeError("Type of argument \"dwProcessId\" must be positive number");
    return;
  }

  HANDLE processHandle = OpenProcess(info[0]->NumberValue(), info[1]->BooleanValue(), info[2]->NumberValue());

  info.GetReturnValue().Set(Nan::New((int)processHandle)->NumberValue());
}

void ExCloseHandle(const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (info.Length() != 1)
  {
    Nan::ThrowTypeError("Wrong Number of Arguments");
    return;
  }

  if (!info[0]->IsNumber())
  {
    Nan::ThrowTypeError("Type of argument \"hObject\" must be positive number");
    return;
  }

  BOOL success = CloseHandle(HANDLE(info[0]->IntegerValue()));

  info.GetReturnValue().Set(Nan::New((bool)success)->ToBoolean());
}
