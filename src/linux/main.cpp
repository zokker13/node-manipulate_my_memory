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
  Set(target, New<String>("processVmReadv").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(NanLinuxProcessVmReadv)).ToLocalChecked());
}

NODE_MODULE(fuck, InitAll)
