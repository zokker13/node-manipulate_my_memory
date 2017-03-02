#ifndef WRITE_PROCESS_MEMORY_HPP_
#define WRITE_PROCESS_MEMORY_HPP_

#include "shared.hpp"

struct WriteProcessMemoryTransformation
{
  WriteProcessMemoryTransformation();
  WriteProcessMemoryTransformation(NAN_METHOD_ARGS_TYPE);
  void FromInfo(NAN_METHOD_ARGS_TYPE);
  void Exec();
  HANDLE hProcess;
  LPVOID lpBaseAddress;
  char *cpBuffer;
  SIZE_T nSize;
  unsigned __int64 uiNUmberOfBytesWritten;
  BOOL bSuccess;
};



class Win32WriteProcessMemory : public AsyncWorker
{
public:
  Win32WriteProcessMemory(Callback* callback, NAN_METHOD_ARGS_TYPE info);
  void Execute();
  void HandleOKCallback();

private:
  WriteProcessMemoryTransformation data;
};

#endif
