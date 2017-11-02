#include "list_processes_and_modules.hpp"

Module::Module(
  string module
  , string exePath
  , uint threadId
  , uint glblUsageCount
  , uint procUsingCount
  , uint modBaseAddress
  , uint modBaseSize
)
  : module(module)
  , exePath(exePath)
  , threadId(threadId)
  , glblUsageCount(glblUsageCount)
  , procUsingCount(procUsingCount)
  , modBaseAddress(modBaseAddress)
  , modBaseSize(modBaseSize)
{
  
}

Process::Process()
  : exeFile("")
  , pid(0)
  , threadCount(0)
  , parentPid(0)
  , priorityClassBase(0)
  , priorityClass(0)
{

}

Process::Process(
  string exeFile
  , uint pid
  , uint threadCount
  , uint parentPid
  , long priorityClassBase
)
  : exeFile(exeFile)
  , pid(pid)
  , threadCount(threadCount)
  , parentPid(parentPid)
  , priorityClassBase(priorityClassBase)
  , priorityClass(0)
{
}

ListProcessesAndModulesTransformation::ListProcessesAndModulesTransformation()
{
}

ListProcessesAndModulesTransformation::ListProcessesAndModulesTransformation(NAN_METHOD_ARGS_TYPE info)
{
  this->FromInfo(info);
}

void ListProcessesAndModulesTransformation::GetProcessList() 
{
	PROCESSENTRY32 pe32;

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hProcessSnap == INVALID_HANDLE_VALUE) 
  {
    throw "Could not create 32Snapshot";
  }

  pe32.dwSize = sizeof(PROCESSENTRY32);

  if (!Process32First(hProcessSnap, &pe32))
  {
    CloseHandle(hProcessSnap);
    throw "Count not get the first Process32";
  }

  do 
  {
    Process proc = Process(
      static_cast<string>(pe32.szExeFile)
      , pe32.th32ProcessID
      , pe32.cntThreads
      , pe32.th32ParentProcessID
      , pe32.pcPriClassBase
    );

    DWORD dwPriorityClass = 0;
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
    if (hProcess == NULL) 
    {
      // cout << "ERR: Could not open process" << endl;
    } 
    else 
    {
      dwPriorityClass = GetPriorityClass(hProcess);
      if (dwPriorityClass)
      {
        proc.priorityClass = dwPriorityClass;
      }
      CloseHandle(hProcess);
    }

    /*
    _tprintf(TEXT("\n  Process ID        = 0x%08X"), pe32.th32ProcessID);
    _tprintf(TEXT("\n  Thread count      = %d"), pe32.cntThreads);
    _tprintf(TEXT("\n  Parent process ID = 0x%08X"), pe32.th32ParentProcessID);
    _tprintf(TEXT("\n  Priority base     = %d"), pe32.pcPriClassBase);
    */

    GetProcessModules(pe32.th32ProcessID, proc);

    processes.push_back(proc);
  } while (Process32Next(hProcessSnap, &pe32));

  CloseHandle(hProcessSnap);
}

void ListProcessesAndModulesTransformation::GetProcessModules(DWORD dwPid, Process &proc)
{
  HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
  MODULEENTRY32 me32;

  hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);

  if (hModuleSnap == INVALID_HANDLE_VALUE)
  {
    // PrintError(TEXT("CreateToolhelp32Snapshot"));
    // cout << "ERR: Could not take snapshot of module with pid" << dwPid << endl;
    return;
  }

  me32.dwSize = sizeof(MODULEENTRY32);

  if (!Module32First(hModuleSnap, &me32))
  {
    PrintError(TEXT("Module32First"));  // show cause of failure
    CloseHandle(hModuleSnap);
    return;
  }

  
  do 
  {
    Module mod = Module(
      static_cast<string>(me32.szModule)
      , static_cast<string>(me32.szExePath)
      , me32.th32ProcessID
      , me32.GlblcntUsage
      , me32.ProccntUsage
      , reinterpret_cast<uint>(me32.modBaseAddr)
      , me32.modBaseSize
    );

    proc.modules.push_back(mod);

    /*
    _tprintf(TEXT("\n\n     MODULE NAME:     %s"), me32.szModule);
    _tprintf(TEXT("\n     Executable     = %s"), me32.szExePath);
    _tprintf(TEXT("\n     Process ID     = 0x%08X"), me32.th32ProcessID);
    _tprintf(TEXT("\n     Ref count (g)  = 0x%04X"), me32.GlblcntUsage);
    _tprintf(TEXT("\n     Ref count (p)  = 0x%04X"), me32.ProccntUsage);
    _tprintf(TEXT("\n     Base address   = 0x%08X"), (DWORD)me32.modBaseAddr);
    _tprintf(TEXT("\n     Base size      = %d"), me32.modBaseSize);
    */
  } while (Module32Next(hModuleSnap, &me32));

  CloseHandle(hModuleSnap);
}

void ListProcessesAndModulesTransformation::Exec()
{
  GetProcessList();
}

void ListProcessesAndModulesTransformation::FromInfo(NAN_METHOD_ARGS_TYPE info)
{

}


Win32ListProcessesAndModules::Win32ListProcessesAndModules(Callback* callback, NAN_METHOD_ARGS_TYPE info)
  : AsyncWorker(callback)
{
  this->data.FromInfo(info);
}

void Win32ListProcessesAndModules::Execute()
{
  this->data.Exec();
}

void Win32ListProcessesAndModules::HandleOKCallback()
{
  Local<Array> procs = Nan::New<Array>(data.processes.size());

  for (int i = 0; i < procs->Length(); i++)
  {
	  Process p = data.processes.at(i);

    Local<Object> hash = Nan::New<Object>();
    Nan::Set(hash, STR("exeFile"), STR(p.exeFile));
    Nan::Set(hash, STR("pid"), Nan::New<Number>(p.pid));
    Nan::Set(hash, STR("threadCount"), Nan::New<Number>(p.threadCount));
    Nan::Set(hash, STR("parentPid"), Nan::New<Number>(p.parentPid));
    Nan::Set(hash, STR("priorityClassBase"), Nan::New<Number>(p.priorityClassBase));
    Nan::Set(hash, STR("priorityClass"), Nan::New<Number>(p.priorityClass));

    Nan::Set(procs, i, hash);
  }

  Local<Value> argv[] = {
    Null()
    , procs
  };

  callback->Call(2, argv);
}

NAN_METHOD(NanWin32ListProcessesAndModules)
{
  Callback *cb = new Callback(info[0].As<Function>());

  AsyncQueueWorker(new Win32ListProcessesAndModules(cb, info));
}

NAN_METHOD(NanWin32ListProcessesAndModulesSync)
{
  ListProcessesAndModulesTransformation trans = ListProcessesAndModulesTransformation(info);
  trans.Exec();

  info.GetReturnValue().Set(Nan::Null());
}
