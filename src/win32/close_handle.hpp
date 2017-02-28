#ifndef CLOSE_HANDLE_HPP_
#define CLOSE_HANDLE_HPP_

#include "shared.hpp"

struct CloseHandleTransformation
{
  CloseHandleTransformation();
  CloseHandleTransformation(NAN_METHOD_ARGS_TYPE);
  void FromInfo(NAN_METHOD_ARGS_TYPE);
  void Exec();
  BOOL bSuccess;
  HANDLE hOpenProcess;
};

class Win32CloseHandle : public AsyncWorker
{
public:
  Win32CloseHandle(Callback *callback, NAN_METHOD_ARGS_TYPE);
  void Execute();
  void HandleOKCallback();

private:
  CloseHandleTransformation data;
};

#endif
