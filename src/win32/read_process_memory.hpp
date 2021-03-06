#ifndef READ_PROCESS_MEMORY_HPP_
#define READ_PROCESS_MEMORY_HPP_

#include "shared.hpp"

struct ReadProcessMemoryTransformation
{
  ReadProcessMemoryTransformation();
  ReadProcessMemoryTransformation(NAN_METHOD_ARGS_TYPE);
  void FromInfo(NAN_METHOD_ARGS_TYPE);
  void Exec();
  HANDLE hProcess;
  LPCVOID lpBaseAddress;
  char *cpBuffer;
  SIZE_T nSize;
  unsigned __int64 uiNUmberOfBytesRead;
  BOOL bSuccess;
};



class Win32ReadProcessMemory : public AsyncWorker
{
public:
  Win32ReadProcessMemory(Callback* callback, NAN_METHOD_ARGS_TYPE info);
  void Execute();
  void HandleOKCallback();

private:
  ReadProcessMemoryTransformation data;
};

#endif
