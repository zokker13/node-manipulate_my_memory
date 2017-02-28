#ifndef GET_WINDOW_THREAD_PROCESS_ID_HPP_
#define GET_WINDOW_THREAD_PROCESS_ID_HPP_

#include "shared.hpp"

struct GetWindowThreadProcessIdTransformation
{
  GetWindowThreadProcessIdTransformation();
  GetWindowThreadProcessIdTransformation(NAN_METHOD_ARGS_TYPE);
  void FromInfo(NAN_METHOD_ARGS_TYPE);
  void Exec();
  HWND hWnd;
  DWORD dwThreadId;
  DWORD dwProcessId;
};

class Win32GetWindowThreadProcessId : public AsyncWorker
{
public:
  Win32GetWindowThreadProcessId(Callback *callback, NAN_METHOD_ARGS_TYPE info);
  void Execute();
  void HandleOKCallback();

private:
  GetWindowThreadProcessIdTransformation data;
};

#endif
