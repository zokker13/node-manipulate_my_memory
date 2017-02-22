#include "windows_extension.h"

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
    GetFunction(New<FunctionTemplate>(ExOpenProcess)).ToLocalChecked());
  Set(target, New<String>("closeHandle").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(ExCloseHandle)).ToLocalChecked());
  Set(target, New<String>("readProcessMemory").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(ExReadProcessMemory)).ToLocalChecked());
}

NODE_MODULE(fuck, InitAll)