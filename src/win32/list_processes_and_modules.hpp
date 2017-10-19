#ifndef READ_PROCESS_MEMORY_HPP_
#define READ_PROCESS_MEMORY_HPP_

#include "shared.hpp"
#include <vector>

typedef unsigned int uint;

class Module 
{
public:
  Module(
    string module
    , string exePath
    , uint threadId
    , uint glblUsageCount
    , uint procUsingCount
    , uint modBaseAddress
    , uint modBaseSize
  );
  string module;
  string exePath;
  uint threadId;
  uint glblUsageCount;
  uint procUsingCount;
  uint modBaseAddress;
  uint modBaseSize;
};

class Process 
{
public:
  Process();
  Process(
    string exeFile
    , uint pid
    , uint threadCount
    , uint parentPid
    , long priorityClassBase
  );
  string exeFile;
  uint pid;
  uint threadCount;
  uint parentPid;
  long priorityClassBase;
  uint priorityClass;
  std::vector<Module> modules;
};

class ListProcessesAndModulesTransformation
{
public:
  ListProcessesAndModulesTransformation();
  ListProcessesAndModulesTransformation(NAN_METHOD_ARGS_TYPE);
  void FromInfo(NAN_METHOD_ARGS_TYPE);
  void Exec();
  vector<Process> processes;

private:
  void GetProcessList();
  void GetProcessModules(DWORD dwPid, Process &proc);
};

class Win32ListProcessesAndModules : public AsyncWorker
{
public:
  Win32ListProcessesAndModules(Callback* callback, NAN_METHOD_ARGS_TYPE info);
  void Execute();
  void HandleOKCallback();

private:
  ListProcessesAndModulesTransformation data;
};

#endif
