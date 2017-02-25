#ifndef WIN32_READ_PROCESS_MEMORY_HPP_
#define WIN32_READ_PROCESS_MEMORY_HPP_

#include "win32_shared.hpp"

struct RpmTransformation
{
  RpmTransformation(NAN_METHOD_ARGS_TYPE info);
  
  HANDLE hProcess;
  LPCVOID lpBaseAddress;
  SIZE_T nSize;
};



class Win32ReadProcessMemory : public AsyncWorker
{
public:
  Win32ReadProcessMemory(Callback* callback, NAN_METHOD_ARGS_TYPE info);
  ~Win32ReadProcessMemory();
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
