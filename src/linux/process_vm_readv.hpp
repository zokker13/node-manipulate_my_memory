#ifndef PROCESS_VM_READV_HPP_
#define PROCESS_VM_READV_HPP_

#include "shared.hpp"

struct ProcessVmReadvTransformation
{
  ProcessVmReadvTransformation();
  ProcessVmReadvTransformation(NAN_METHOD_ARGS_TYPE);
  ~ProcessVmReadvTransformation();
  void FromInfo(NAN_METHOD_ARGS_TYPE);
  void Exec();
  pid_t pid;
  void *address;
  char *buffer;
  int size;
  ssize_t uiNUmberOfBytesRead;
  bool bSuccess;
};



class LinuxProcessVmReadv : public AsyncWorker
{
public:
  LinuxProcessVmReadv(Callback* callback, NAN_METHOD_ARGS_TYPE info);
  void Execute();
  void HandleOKCallback();

private:
  ProcessVmReadvTransformation data;
};

#endif
