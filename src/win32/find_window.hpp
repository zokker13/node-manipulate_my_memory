#ifndef FIND_WINDOW_HPP_
#define FIND_WINDOW_HPP_

#include "shared.hpp"

struct FindWindowTransformation
{
  FindWindowTransformation();
  ~FindWindowTransformation();
  FindWindowTransformation(NAN_METHOD_ARGS_TYPE);
  void FromInfo(NAN_METHOD_ARGS_TYPE);
  void Exec();
  HWND handle;
  char *lpClassName;
  char *lpWindowName;
};

class Win32FindWindow : public AsyncWorker
{
public:
  Win32FindWindow(Callback *callback, NAN_METHOD_ARGS_TYPE info);
  void Execute();
  void HandleOKCallback();

private:
  FindWindowTransformation data;
};


#endif
