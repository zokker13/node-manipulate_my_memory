#include "main.hpp"

using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;

NAN_MODULE_INIT(InitAll)
{
  Set(target, New<String>("openProcess").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32OpenProcess)).ToLocalChecked());
  Set(target, New<String>("openProcessSync").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32OpenProcessSync)).ToLocalChecked());
  Set(target, New<String>("closeHandle").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32CloseHandle)).ToLocalChecked());
  Set(target, New<String>("closeHandleSync").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32CloseHandleSync)).ToLocalChecked());
  Set(target, New<String>("readProcessMemory").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32ReadProcessMemory)).ToLocalChecked());
  Set(target, New<String>("readProcessMemorySync").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32ReadProcessMemorySync)).ToLocalChecked());
  Set(target, New<String>("writeProcessMemory").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32WriteProcessMemory)).ToLocalChecked());
  Set(target, New<String>("writeProcessMemorySync").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32WriteProcessMemorySync)).ToLocalChecked());
  Set(target, New<String>("findWindow").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32FindWindow)).ToLocalChecked());
  Set(target, New<String>("findWindowSync").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32FindWindowSync)).ToLocalChecked());
  Set(target, New<String>("getWindowThreadProcessId").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32GetWindowThreadProcessId)).ToLocalChecked());
  Set(target, New<String>("getWindowThreadProcessIdSync").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32GetWindowThreadProcessIdSync)).ToLocalChecked());
  Set(target, New<String>("listProcessesAndModules").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32ListProcessesAndModules)).ToLocalChecked());
  Set(target, New<String>("listProcessesAndModulesSync").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32ListProcessesAndModulesSync)).ToLocalChecked());
}

NODE_MODULE(fuck, InitAll)
