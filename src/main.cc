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
  Set(target, New<String>("closeHandle").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32CloseHandle)).ToLocalChecked());
  Set(target, New<String>("readProcessMemory").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanWin32ReadProcessMemory)).ToLocalChecked());
}

NODE_MODULE(fuck, InitAll)
