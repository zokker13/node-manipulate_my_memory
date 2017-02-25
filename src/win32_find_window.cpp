#include "win32_find_window.hpp"

FindWindowTransformation::FindWindowTransformation()
{
  this->handle = nullptr;
  this->lpClassName = nullptr;
  this->lpWindowName = nullptr;
}

FindWindowTransformation::~FindWindowTransformation()
{
  if (this->lpWindowName != nullptr)
    delete[] this->lpWindowName;

  if (this->lpClassName != nullptr)
    delete[] this->lpClassName;
}

FindWindowTransformation::FindWindowTransformation(NAN_METHOD_ARGS_TYPE info)
{
  this->FromInfo(info);
}

void FindWindowTransformation::Exec()
{ 
  this->handle = FindWindow(this->lpClassName, this->lpWindowName);
}

void FindWindowTransformation::FromInfo(NAN_METHOD_ARGS_TYPE info)
{
  if (info[0]->IsNull())
    this->lpClassName = nullptr;

  else
  {
    v8::String::Utf8Value tempStr(info[0]->ToString());
    this->lpClassName = new char[tempStr.length() + 1];
    memcpy(this->lpClassName, *tempStr, tempStr.length());
    this->lpClassName[tempStr.length()] = 0;
  }

  
  if (info[1]->IsNull())
    this->lpWindowName = nullptr;

  else
  {
    v8::String::Utf8Value tempStr(info[1]->ToString());
    this->lpWindowName = new char[tempStr.length() + 1];
    memcpy(this->lpWindowName, *tempStr, tempStr.length());
    this->lpWindowName[tempStr.length()] = 0;
  }
  

}

Win32FindWindow::Win32FindWindow(Callback* callback, NAN_METHOD_ARGS_TYPE info)
  : AsyncWorker(callback)
{
  this->data.FromInfo(info);
}

void Win32FindWindow::Execute()
{
  this->data.Exec();
}

void Win32FindWindow::HandleOKCallback()
{
  HandleScope scope;

  Local<Value> argv[] = {
    Null()
    , New<Number>(reinterpret_cast<int>(this->data.handle))
  };

  callback->Call(2, argv);
}

NAN_METHOD(NanWin32FindWindow)
{
  Callback *cb = new Callback(info[2].As<Function>());

  AsyncQueueWorker(new Win32FindWindow(cb, info));
}

NAN_METHOD(NanWin32FindWindowSync)
{
  FindWindowTransformation trans = FindWindowTransformation(info);
  trans.Exec();
  info.GetReturnValue().Set(reinterpret_cast<int>(trans.handle));
}


