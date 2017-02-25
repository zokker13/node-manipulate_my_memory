#include "windows_extension.hpp"

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
  Set(target, New<String>("findWindow").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32FindWindow)).ToLocalChecked());
  Set(target, New<String>("findWindowSync").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32FindWindowSync)).ToLocalChecked());
}

NODE_MODULE(fuck, InitAll)
